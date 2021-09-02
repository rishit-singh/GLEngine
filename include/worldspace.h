#include "globals.h"
#include "globjects.h"

namespace GLEngine
{
		struct WorldSpace
	{
		glm::mat4 View { glm::mat4(1.0f) }, 
			Projection { glm::mat4(1.0f) };

		WorldSpace()
		{	
		}
		
		WorldSpace(glm::mat4 view, glm::mat4 projection) : View(view), Projection(projection)
		{
		}
	};
}