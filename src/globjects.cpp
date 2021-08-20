#include "globjects.h"
#include "window.h"

String* GLEngine::DefaultStringValues = new String[1] {
	"New Window"
}; 

GLEngine::Point2D* GLEngine::DefaultResolutions = new Point2D[1] {
	Point2D(800, 600)	
};

std::vector<float> GLEngine::Vertex3Df::GetSingleBuffer()
{
	this->SingleBuffer.push_back(this->Position.X);
	this->SingleBuffer.push_back(this->Position.Y);
	this->SingleBuffer.push_back(this->Position.Z);
	
	this->SingleBuffer.push_back(this->TextureCoordinates.X);
	this->SingleBuffer.push_back(this->TextureCoordinates.Y);

	this->SingleBuffer.push_back(this->Normals.X);
	this->SingleBuffer.push_back(this->Normals.Y);
	this->SingleBuffer.push_back(this->Normals.Z);

	return this->SingleBuffer;
}
