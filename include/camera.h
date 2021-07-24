#ifndef CAMERA_H

#include "Debug.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

namespace GLEngine
{
	struct CameraAxis	//	Stores the camera axis
	{	
		glm::vec3 Front,	//	Front axis
				Up;		// Up axis

		CameraAxis(glm::vec3 front, glm::vec3 up) : Front(front), Up(up) 
		{
		}	
	};

	class Camera	//	Rendering camera
	{
	public:
		unsigned int ID;

		glm::vec3 Position,	//	Camera postion vector.
				Target,		//	Camera target
				Direction;	//	Camera direction vector

		CameraAxis Axis;	//	CameraAxis instance.

		bool IsValid();
	
		glm::mat4 GetViewMatrix();

		Camera(unsigned int id, glm::vec3 position, glm::vec3 target, glm::vec3 direction, CameraAxis cameraAxis) : ID(id), Position(position), Target(target), Direction(direction), Axis(cameraAxis)
		{			
		}

		~Camera()
		{
		}
	};
}

#endif
