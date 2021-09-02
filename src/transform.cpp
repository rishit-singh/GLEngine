#include "transform.h"

void GLEngine::Transform::Move(float n, GLEngine::Axis axis)
{
	switch (axis)
	{
		case GLEngine::Axis::X:
			this->Position.X += n;

			break;

		case GLEngine::Axis::Y:
			this->Position.Y += n;

			break;

		case GLEngine::Axis::Z:
			this->Position.Z += n;

			break;
	}


	this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(this->Position.X, this->Position.Y, this->Position.Z));
}

void GLEngine::Transform::Rotate(float angle, Point3Df axis)
{
	this->Reset();

	this->ModelMatrix = glm::rotate(this->ModelMatrix, angle, glm::vec3(axis.X, axis.Y, axis.Z));
}

glm::mat4* GLEngine::Transform::GetModelMatrix()
{
	return &this->ModelMatrix;
}

void GLEngine::Transform::Reset()
{
	this->ModelMatrix = glm::mat4(1.0f);
	
	this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(this->Position.X, this->Position.Y, this->Position.Z));
}
