#include "globals.h"

Debugger* GLEngine::Debug = new Debugger(); 

char GLEngine::DefaultPaths[2][2][1024] = {
	{
		"shaders/vertexshader.vert",
		"shaders/fragmentshader.frag"
	},
	
	{
		"resources/dirt.jpg"
	}
};
