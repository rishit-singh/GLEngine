#include "blending.h"

void GLEngine::Blender::Enable()
{
	glEnable(GL_BLEND);
}

void GLEngine::Blender::Disable()
{
	glEnable(GL_BLEND);
}

void GLEngine::Blender::ApplyBlend(GLenum source, GLenum destination)
{
	glBlendFunc(source, destination);
}
