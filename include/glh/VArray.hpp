#pragma once

#include <GL/glew.h>

namespace glh {

class Buffer;

class VArray {
public:
	VArray();
	VArray(const VArray& other) = delete;
	VArray& operator=(const VArray& other) = delete;
	VArray(VArray&& other);
	VArray& operator=(VArray&& other);
	~VArray();

	void bind() const;
	static void unbind();

	void enableVertexAttrib(GLuint index);
	void disableVertexAttrib(GLuint index);

	void vertexAttribPointer(const Buffer& b, GLuint index, GLint size, GLenum type,
			         GLboolean normalized = GL_FALSE,
				 GLsizei stride = 0,
				 GLintptr offset = 0);

private:
	GLuint name_;
};

}
