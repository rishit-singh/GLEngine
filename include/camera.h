#ifndef CAMERA_H

#include "Debug.h"
#include "external/glm/glm.hpp"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

namespace GLEngine
{
	struct CameraAxis	//	Stores the camera axis
	{	
		glm::vec3 Front { glm::vec3() },	
				Up { glm::vec3() };
		
		float Pitch { 0.0f },		
				Yaw { 0.0f };	//	Euler angles

		CameraAxis(glm::vec3 front, glm::vec3 up, float pitch, float yaw) : Front(front), Up(up), Pitch(pitch), Yaw(yaw)
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

		float FOV { 45.0f };	//	Camera FOV
		
		float Senstivity { 0.1f };	//	Camera senstivity
		
		bool IsValid();
	
		glm::mat4 GetViewMatrix();

		Camera(unsigned int id, glm::vec3 position, glm::vec3 target, glm::vec3 direction, CameraAxis cameraAxis, float fov) : ID(id), Position(position), Target(target), Direction(direction), Axis(cameraAxis), FOV(fov)
		{			
		}

		~Camera()
		{
		}
	};
}

#endif
