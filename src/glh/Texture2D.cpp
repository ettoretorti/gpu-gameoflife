#include <glh/Texture2D.hpp>
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
		glBindTexture(GL_TEXTURE_2D, name);
		binds[curTexUnit] = name;
	}
}

Texture2D::Texture2D(GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height) : name_(0)
{
	if(GLEW_ARB_direct_state_access && GLEW_ARB_texture_storage) {
		glCreateTextures(GL_TEXTURE_2D, 1, &name_);
		glTextureStorage2D(name_, levels, internalformat, width, height);
	} else if(GLEW_ARB_texture_storage) {
		glGenTextures(1, &name_);
		texBind(name_);
		glTexStorage2D(GL_TEXTURE_2D, levels, internalformat, width, height);
	} else {
		glGenTextures(1, &name_);
		texBind(name_);
		for(int i = 0; i < levels; i++) {
			glTexImage2D(GL_TEXTURE_2D, i, internalformat, width, height, 0,
			             internalformat == GL_STENCIL_INDEX ? GL_STENCIL_INDEX : GL_RGB,
			             GL_BYTE, nullptr);
			width = std::max(1, width / 2);
			height = std::max(1, height / 2);
		}
	}
}
Texture2D::Texture2D(Texture2D&& other) : name_(0) {
	auto tmp = name_;
	name_ = other.name_;
	other.name_ = tmp;
}

Texture2D& Texture2D::operator=(Texture2D&& other) {
	auto tmp = name_;
	name_ = other.name_;
	other.name_ = tmp;
	
	return *this;
}

Texture2D::~Texture2D() {
	glDeleteTextures(1, &name_);
	if(curBound() == name_) {
		texBind(0);
	}
}
	
void Texture2D::subImage(GLsizei width, GLsizei height, GLenum format,
                         GLenum type, const GLvoid* pixels,
                         GLint level, GLint xoffset, GLint yoffset) {
	if(GLEW_ARB_direct_state_access) {
		glTextureSubImage2D(name_, level, xoffset, yoffset, width, height, format, type, pixels);
	} else {
		texBind(name_);
		glTexSubImage2D(GL_TEXTURE_2D, level, xoffset, yoffset, width, height, format, type, pixels);
	}
}

void Texture2D::generateMipmaps() const {
	if(GLEW_ARB_direct_state_access) {
		glGenerateTextureMipmap(name_);
	} else {
		texBind(name_);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Texture2D::parameter(GLenum pname, GLint param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameteri(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameteri(GL_TEXTURE_2D, pname, param);
	}
}

void Texture2D::parameter(GLenum pname, GLfloat param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameterf(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameterf(GL_TEXTURE_2D, pname, param);
	}
}

void Texture2D::parameterv(GLenum pname, const GLint* param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameteriv(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameteriv(GL_TEXTURE_2D, pname, param);
	}
}

void Texture2D::parameterv(GLenum pname, const GLfloat* param) {
	if(GLEW_ARB_direct_state_access) {
		glTextureParameterfv(name_, pname, param);
	} else {
		texBind(name_);
		glTexParameterfv(GL_TEXTURE_2D, pname, param);
	}
}

GLuint Texture2D::name() const {
	return name_;
}

void Texture2D::bind() const {
	texBind(name_);
}

void Texture2D::unbind() {
	texBind(0);
}

}
