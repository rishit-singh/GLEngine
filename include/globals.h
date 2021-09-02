#pragma once 

#define GLOBALS_H

#include "Debug.h"

#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

#define ASSERT(x) if (!(x)) raise(SIGINT);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

using namespace DebugTools;

namespace GLEngine
{
	enum ResourceType 
	{
		Shaders,
		Textures
	};

	extern Debugger* Debug;	//	global Debugger instance 

	extern char DefaultPaths[2][2][1024];	//	Default paths for the engine resources
};

