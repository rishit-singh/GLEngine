#include "camera.h"

glm::mat4 GLEngine::Camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Axis.Front, this->Axis.Up);
}

