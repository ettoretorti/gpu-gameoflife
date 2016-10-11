#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <mathfu/glsl_mappings.h>

#include <glh/Buffer.hpp>
#include <glh/VArray.hpp>
#include <glh/TextureUnit.hpp>
#include <glh/Texture1D.hpp>
#include <glh/Texture2D.hpp>
#include <glh/Shader.hpp>
#include <glh/Program.hpp>

#include <iostream>
#include <random>

const GLchar* gameVShader =
	"#version 330 core\n"
	"\n"
	"layout(location = 0) in vec2 pos;\n"
	"\n"
	"out vec2 f_uv;\n"
	"\n"
	"void main() {\n"
	"    gl_Position = vec4(pos, 0.0, 1.0);\n"
	"    f_uv = (pos + 1.0) * 0.5;\n"
	"}\n";

const GLchar* gameFShader = 
	"#version 330 core\n"
	"\n"
	"uniform vec2 OFFSETS\n;"
	"uniform sampler2D PREVIOUS\n;"
	"\n"
	"in vec2 f_uv;\n"
	"\n"
	"layout(location = 0) out float alive;\n"
	"\n"
	"void main() {\n"
	"    float acc = 0.0;\n"
	"    for(int j = -1; j < 2; j++) {\n"
	"        for(int i = -1; i < 2; i++) {\n"
	"            acc += texture(PREVIOUS, f_uv + vec2(i, j) * OFFSETS).r;\n"
	"        }\n"
	"    };\n"
	"\n"
	"    float dis = texture(PREVIOUS, f_uv).r;\n"
	"    alive = (acc == 3.0 ||\n"
	"             acc == 4.0 && dis == 1.0) ? 1.0\n"
	"                                        : 0.0;\n"
	"}\n";

const GLchar* renderFShader =
	"#version 330 core\n"
	"\n"
	"uniform sampler2D BOARD;\n"
	"\n"
	"in vec2 f_uv;\n"
	"\n"
	"layout(location = 0) out vec4 color;\n"
	"\n"
	"void main() {\n"
	"    color = vec4(texture(BOARD, f_uv).r);\n"
	"}\n";

bool compileShader(glh::Shader& shader, const char* name) {
	if(!shader.compile()) {
		std::cerr << "Could not compile " << name << " shader" << std::endl;
		std::cerr << shader.infoLog() << std::endl;
		return false;
	}

	return true;
}

bool linkProgram(glh::Program& p, const char* name) {
	if(!p.link()) {
		std::cerr << "Could not link " << name << " program" << std::endl;
		std::cerr << p.infoLog() << std::endl;
		return false;
	}

	return true;
}

void mainloop(SDL_Window* win, int width, int height, float timePerTurn) {
	using namespace std;
	using namespace glh;
	using namespace mathfu;

	GLfloat coords[] = {
		-1.0, -1.0,
		 1.0, -1.0,
		 1.0,  1.0,
		-1.0,  1.0
	};
	Buffer twotrigs(GL_ARRAY_BUFFER);
	
	twotrigs.data(4 * 2 * sizeof(GLfloat), coords, GL_STATIC_DRAW);

	VArray vao;
	vao.enableVertexAttrib(0);
	vao.vertexAttribPointer(twotrigs, 0, 2, GL_FLOAT);
	
	Shader vs(GL_VERTEX_SHADER);
	vs.source(gameVShader);
	if(!compileShader(vs, "game vertex")) return;
	
	Shader fs(GL_FRAGMENT_SHADER);
	fs.source(gameFShader);
	if(!compileShader(fs, "game fragment")) return;
	
	Shader rfs(GL_FRAGMENT_SHADER);
	rfs.source(renderFShader);
	if(!compileShader(rfs, "render fragment")) return;
	
	Program pg;
	pg.setVertexShader(vs);
	pg.setFragmentShader(fs);
	if(!linkProgram(pg, "game")) return;

	Program pr;
	pr.setVertexShader(vs);
	pr.setFragmentShader(rfs);
	if(!linkProgram(pr, "render")) return;

	vec2i size;
	SDL_GetWindowSize(win, &size.x(), &size.y());

	Texture2D front(1, GL_R8, width, height);
	front.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	front.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	front.parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	front.parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	Texture2D back(1, GL_R8, width, height);
	back.parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	back.parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	back.parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	back.parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//black out window while randomizing
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);
	{
		GLubyte* data = new GLubyte[width * height];
		std::random_device randd;
		for(int i = 0; i < width * height; i++) {
			data[i] = randd() < (unsigned int)(0.3 * 4294967295) ? 255 : 0;
		}
		front.subImage(width, height, GL_RED, GL_UNSIGNED_BYTE, data);
		delete[] data;
	}

	Uint32 time = SDL_GetTicks();
	int timeToUpdate = int(timePerTurn * 1000);
#ifndef NDEBUG
	int acc = 0;
	unsigned char counter = 0;
#endif
	while(true) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
			case SDL_QUIT: {
				return;
			}
			case SDL_WINDOWEVENT: {
				if(e.window.event == SDL_WINDOWEVENT_RESIZED) {
					size.x() = e.window.data1;
					size.y() = e.window.data2;
				}
				break;
			}
			}
		}

		Uint32 cTime = SDL_GetTicks();
		timeToUpdate -= (cTime - time);
#ifndef NDEBUG
		acc += (cTime - time);
		if(++counter == 0) {
			cout << "AVG frame time: " << (acc / 256.0) << endl;
			acc = 0;
		}
#endif
		time = cTime;
		
		if(timeToUpdate <= 0) {
			timeToUpdate += int(timePerTurn * 1000);
			
			//-----------------
			// Calculate state
			//-----------------
			std::swap(front, back);

			GLuint fbuffer;
			glGenFramebuffers(1, &fbuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, fbuffer);
			
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, front.name(), 0);
			GLenum drawBufs = GL_COLOR_ATTACHMENT0;
			glDrawBuffers(1, &drawBufs);

			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) cerr << "WOW" << endl;
			glViewport(0, 0, width, height);

			//drawing is ready...
			pg.use();
			glUniform2f(pg.getUniform("OFFSETS"), 1.0 / width, 1.0 / height);
			vao.bind();
			back.bind();
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glDeleteFramebuffers(1, &fbuffer);
		}
		
		//----------------
		//Draw final frame
		//----------------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, size.x(), size.y());
		pr.use();
		front.bind();
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		SDL_GL_SwapWindow(win);
	}
	
}
