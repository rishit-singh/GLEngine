#pragma once 

#define GLENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include "renderer.h"

using namespace DebugTools; 

namespace GLEngine
{
	extern void SetWindowHints(unsigned int, unsigned int);	//	Sets up the OpenGL version hint
	extern void SetCurrentContext(Window);	//	Sets the provided instance of a GLFWwindow as the current context
	extern bool SetupGLEW();	//	Sets up the GLEW library
	extern bool SetupGLFW();	//	Sets up the GLFW library
	
	extern GLenum* GLObjectEnums; // Stores rendering type GLenums for different objects
}
