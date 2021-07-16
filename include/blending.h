#ifndef BLENDING_H

#include <GL/glew.h>

namespace GLEngine
{
	class Blender
	{
	public:
		void Enable();  //  Enables blending
		void Disable(); //  Disables blending
		void ApplyBlend(GLenum, GLenum);    //  Set the parameters of the blend equation.
	};
}

#endif
