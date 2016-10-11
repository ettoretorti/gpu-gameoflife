#include <glh/VArray.hpp>

#include <glh/Buffer.hpp>
#include <GL/glew.h>
#include <utility>

namespace glh {

//opengl state shadowing for performance
static thread_local GLuint curVAO = 0;
static void vaoBind(GLuint name) {
	if(curVAO != name) {
		glBindVertexArray(name);
		curVAO = name;
	}
}

VArray::VArray() : name_(0)
{
	if(GLEW_ARB_direct_state_access) {
		glCreateVertexArrays(1, &name_);
	} else {
		glGenVertexArrays(1, &name_);
	}
}

VArray::VArray(VArray&& other) : name_(other.name_)
{
	other.name_ = 0;
}

VArray& VArray::operator=(VArray&& other) {
	std::swap(name_, other.name_);
	return *this;
}

VArray::~VArray() {
	if (curVAO == name_) {
		curVAO = 0;
	}
	glDeleteVertexArrays(1, &name_);
}

void VArray::bind() const {
	vaoBind(name_);
}

void VArray::unbind() {
	vaoBind(0);
}

void VArray::enableVertexAttrib(GLuint index) {
	if(GLEW_ARB_direct_state_access) {
		glEnableVertexArrayAttrib(name_, index);
		return;
	}

	vaoBind(name_);
	glEnableVertexAttribArray(index);
}

void VArray::disableVertexAttrib(GLuint index) {
	if(GLEW_ARB_direct_state_access) {
		glDisableVertexArrayAttrib(name_, index);
		return;
	}

	vaoBind(name_);
	glDisableVertexAttribArray(index);
}

void VArray::vertexAttribPointer(const Buffer& b, GLuint index, GLint size, GLenum type,
                                   GLboolean normalized, GLsizei stride, GLintptr offset) {
	Buffer& buf = const_cast<Buffer&>(b);

	vaoBind(name_);
	GLuint oldT = buf.target();
	buf.target() = GL_ARRAY_BUFFER;
	buf.bind();
	glVertexAttribPointer(index, size, type, normalized, stride, (void*)offset);
	buf.target() = oldT;
}

}
