#pragma once

#include <GL/glew.h>

namespace glh {

//Every function can cause a bind to the current texture unit. Tread carefully
class Texture2D {
public:
	Texture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
	Texture2D(const Texture2D& other) = delete;
	Texture2D& operator=(const Texture2D& other) = delete;
	Texture2D(Texture2D&& other);
	Texture2D& operator=(Texture2D&& other);
	~Texture2D();
	
	void subImage(GLsizei width, GLsizei height, GLenum format,
	              GLenum type, const GLvoid* pixels,
	              GLint level = 0, GLint xoffset = 0, GLint yoffset = 0);
	void generateMipmaps() const;
	void parameter(GLenum pname, GLint param);
	void parameter(GLenum pname, GLfloat param);
	void parameterv(GLenum pname, const GLint* param);
	void parameterv(GLenum pname, const GLfloat* param);

	void bind() const;
	static void unbind();
	
	GLuint name() const;
private:
	GLuint name_;
};

}
