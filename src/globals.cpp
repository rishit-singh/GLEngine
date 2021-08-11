#include "globals.h"

Debugger* GLEngine::Debug = new Debugger(); 

char GLEngine::DefaultPaths[2][2][1024] = {
	{
		"/media/rishit/HDD0/src/repos/GLEngine/shaders/vertexshader.vert",
		"/media/rishit/HDD0/src/repos/GLEngine/shaders/fragmentshader.frag"
	},
	
	{
		"/media/rishit/HDD0/src/repos/GLEngine/resources/dirt.jpg"
	}
};
