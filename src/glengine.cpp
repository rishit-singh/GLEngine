#include "glengine.h"
#include "exceptions.h"

GLenum* GLEngine::GLObjectEnums = new GLenum[1] {
	GL_TRIANGLES
}; 


void GLEngine::SetWindowHints(unsigned int major, unsigned int minor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__	//	macOS support
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
} 

void GLEngine::SetCurrentContext(Window window)
{	
	glfwMakeContextCurrent(window.GLWindow);
}

bool GLEngine::SetupGLEW()
{
	glewExperimental = GL_TRUE; 

	try
	{
		if (glewInit() != GLEW_OK)
			throw new LibraryInitializationError("GLEW");
	}
	catch (const LibraryInitializationError& e)
	{	
		std::cout << "\nGLEW initialization error.";

		return false; 
	}
	
	return true;
}

bool GLEngine::SetupGLFW()
{
	if (!glfwInit())
	{
		std::cout << "\nGLFW initialization error.";

		return false; 
	}

	return true;
}


void GLEngine::VertexArrayObject::Unbind()
{
	glBindVertexArray(0);
	this->IsBound = false;
}

