#include <glh/Texture1D.hpp>
#include <glh/TextureUnit.hpp>

#include <GL/glew.h>

#include <algorithm>

namespace glh {

static thread_local GLuint binds[80] = {0};

static GLuint curBound() {
	return binds[TextureUnit::active() - GL_TEXTURE0];
}
static void texBind(GLuint name) {
	GLuint curTexUnit = curBound();
	if(binds[curTexUnit] != name) {
		glBindTexture(GL_TEXTURE_1D, name);
		binds[curTexUnit] = name;
	}
}

Texture1D::Texture1D(GLsizei levels, GLenum internalformat, GLsizei width) : name_(0)
{
	if(GLEW_ARB_direct_state_access && GLEW_ARB_texture_storage) {
		glCreateTextures(GL_TEXTURE_1D, 1, &name_);
		glTextureStorage1D(name_, levels, internalformat, width);
	} else if(GLEW_ARB_texture_storage) {
		glGenTextures(1, &name_);
		texBind(name_);
		glTexStorage1D(GL_TEXTURE_1D, levels, internalformat, width);
	} else {
		glGenTextures(1, &name_);
		texBind(name_);
		for(int i = 0; i < levels; i++) {
			glTexImage1D(GL_TEXTURE_1D, i, internalformat, width, 0,
			             internalformat == GL_STENCIL_INDEX ? GL_STENCIL_INDEX : GL_RGB,
			             GL_BYTE, nullptr);
			width = std::max(1, width / 2);
		}
	}
}
Texture1D::Texture1D(Texture1D&& other) : name_(0) {
	auto tmp = name_;
	name_ = other.name_;
	other.name_ = tmp;
}

Texture1D& Texture1D::operator=(Texture1D&& other) {
	auto tmp = name_;
	name_ = other.name_;
	other.name_ = tmp;
	
	return *this;
}

Texture1D::~Texture1D() {
	glDeleteTextures(1, &name_);
	if(curBound() == name_) {
		texBind(0);
	}
}
	
void Texture1D::subImage(GLsizei width, GLenum format,
                         GLenum type, const GLvoid* pixels,
                         GLint level, GLint xoffset) {
	if(GLEW_ARB_direct_state_access) {
		glTextureSubImage1D(name_, level, xoffset, width, format, type, pixels);
	} else {
		texBind(name_);
		glTexSubImage1D(GL_TEXTURE_1D, level, xoffset, width, format, type, pixels);
	}
}

void Texture1D::generateMipmaps() const {
	if(GLEW_ARB_direct_state_access) {
		glGenerateTextureMipmap(name_);
	} else {
		texBind(name_);
		glGenerateMipmap(GL_TEXTURE_1D);
	}
}

void Texture1D::parameter(GLenum pname, GLint param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameteri(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameteri(GL_TEXTURE_1D, pname, param);
	}
}

void Texture1D::parameter(GLenum pname, GLfloat param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameterf(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameterf(GL_TEXTURE_1D, pname, param);
	}
}

void Texture1D::parameterv(GLenum pname, const GLint* param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameteriv(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameteriv(GL_TEXTURE_1D, pname, param);
	}
}

void Texture1D::parameterv(GLenum pname, const GLfloat* param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameterfv(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameterfv(GL_TEXTURE_1D, pname, param);
	}
}

GLuint Texture1D::name() const {
	return name_;
}

void Texture1D::bind() const {
	texBind(name_);
}

void Texture1D::unbind() {
	texBind(0);
}

}
