#include <glh/TextureUnit.hpp>

#include <GL/glew.h>

namespace glh { namespace TextureUnit {

static thread_local GLenum activeUnit = GL_TEXTURE0;

GLenum active() {
	return activeUnit;
}

void setActive(GLenum unit) {
	if(activeUnit != unit) {
		glActiveTexture(unit);
	}
}

}}
