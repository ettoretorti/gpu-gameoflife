#pragma once

#include <GL/glew.h>

namespace glh {

class Buffer {
	friend class VArray;

public:
	Buffer(GLenum target = GL_ARRAY_BUFFER);
	Buffer(const Buffer& other) = delete;
	Buffer& operator=(const Buffer& other) = delete;
	Buffer(Buffer&& other);
	Buffer& operator=(Buffer&& other);
	~Buffer();

	GLenum& target();
	const GLenum& target() const;
	
	void bind() const;
	static void unbind(GLenum target);

	void data(GLsizeiptr size, const GLvoid* data, GLenum usage);
	void subData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

private:
	GLuint name_;
	GLenum target_;
};

}
