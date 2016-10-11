#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <mathfu/glsl_mappings.h>

#include <iostream>
#include <string>


#ifndef NDEBUG
  #ifndef APIENTRY
    #define APIENTRY
  #endif

void APIENTRY gl_error(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* uParam) {
	(void)src; (void)type; (void)id; (void)severity; (void)length; (void)uParam;
	std::cerr << msg << std::endl;
}

#endif

void mainloop(SDL_Window* win, int width, int height, float time);

int main(int argc, char** argv) {
	using namespace std;

	int width = 512;
	int height = 512;
	float timePerTurn = 0.1;

	if(argc > 2) {
		try {
			int uWidth  = stoi(string(argv[1]));
			int uHeight = stoi(string(argv[2]));

			if(uWidth > 0 && uHeight > 0) {
				width = uWidth;
				height = uHeight;
			}
		} catch(...) {
			std::cerr << "Could not parse dimensions." << std::endl;
		}

	}

	if(argc > 3) {
		try {
			float uTimePerTurn = stof(string(argv[3]));

			if(uTimePerTurn > 0.0) {
				timePerTurn = uTimePerTurn;
			}
		} catch(...) {
			std::cerr << "Could not parse time per turn." << std::endl;
		}
	}

	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		cerr << "Failed to init SDL" << endl;
		cerr << SDL_GetError() << endl;
		return -1;	
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
#ifndef NDEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	SDL_Window* win = SDL_CreateWindow(
	                      "game of life",
	                      SDL_WINDOWPOS_UNDEFINED,
	                      SDL_WINDOWPOS_UNDEFINED,
	                      512,
	                      512,
	                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if(!win) {
		cerr << "Failed to create window" << endl;
		cerr << SDL_GetError() << endl;
		return -1;
	}

	SDL_GLContext glc = SDL_GL_CreateContext(win);
	if(!glc) {
		cerr << "Failed to create OpenGL context" << endl;
		cerr << SDL_GetError() << endl;
		return -1;
	}
	
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if(err != GLEW_OK) {
		cerr << "Could not init GLEW" << endl;
		cerr << glewGetErrorString(err) << endl;
		return -1;
	}

#ifndef NDEBUG
	if (GLEW_KHR_debug) {
		glDebugMessageCallback(gl_error, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		std::cout << "Registered gl debug callback" << std::endl;
	}
#endif

	//ready
	mainloop(win, width, height, timePerTurn);

	SDL_GL_DeleteContext(glc);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
