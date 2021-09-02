#ifndef TRANFORM_H

#include "globals.h"
#include "globjects.h"

namespace GLEngine
{
	enum class Axis
	{
		X,
		Y,
		Z	
	};

	class Transform	//	Stores the transform information and the required model matrix of an object.
	{
	private:
		glm::mat4 ModelMatrix { glm::mat4(1.0f) };	//	Model matrix instance

	public:
		Point3Df Position,	//	3D world space position of the current Transform instace.
				RotationAxis;

		float RotationAngle { 0.0f };	//	Default angle in degrees at which the current Transform instance will rotate at.

		glm::mat4* GetModelMatrix();	//	Returns the ModelMatrix in the current state

		void Move(float, Axis);		//	Adds the provided value to the transform at the provided axis
		void Rotate(Point3Df);	//	Rotates the current transform around the provided axis at the default rotation angle
		void Rotate(float, Point3Df);	//	Rotates the current transform around the provided axis at the provided rotation angle 

		void Reset();	//	Resets the current transform

		Transform() : ModelMatrix(glm::mat4(1.0f))
		{
			this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(this->Position.X, this->Position.Y, this->Position.Z));
		}

		Transform(Point3Df position) : Position(position), ModelMatrix(glm::mat4(1.0f))
		{
			this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(this->Position.X, this->Position.Y, this->Position.Z));
		}

		Transform(Point3Df position, float angle) : Position(position), RotationAngle(angle), ModelMatrix(glm::mat4(1.0f))
		{
			this->ModelMatrix = glm::translate(this->ModelMatrix, glm::vec3(this->Position.X, this->Position.Y, this->Position.Z));
		}
	};
}

#endif
