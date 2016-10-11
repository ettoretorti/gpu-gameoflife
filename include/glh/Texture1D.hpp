#pragma once

#include <GL/glew.h>

namespace glh {

//Every function can cause a bind to the current texture unit. Tread carefully
class Texture1D {
public:
	Texture1D(GLsizei levels, GLenum internalformat, GLsizei width);
	Texture1D(const Texture1D& other) = delete;
	Texture1D& operator=(const Texture1D& other) = delete;
	Texture1D(Texture1D&& other);
	Texture1D& operator=(Texture1D&& other);
	~Texture1D();
	
	void subImage(GLsizei width, GLenum format,
	              GLenum type, const GLvoid* pixels,
	              GLint level = 0, GLint xoffset = 0);
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
