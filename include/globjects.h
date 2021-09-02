#pragma once

#define GLOBJECTS_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
#include "gltypes.h"
#include "Debug.h"

using namespace DebugTools;

namespace GLEngine
{	
	struct Point2D
	{	
		int X, Y; 

		Point2D() : X(0), Y(0)
		{
		}

		Point2D(int x, int y) : X(x), Y(y)
		{
		}

		Point2D operator +(const Point2D& rhs)
		{
			return Point2D(Point2D(this->X + rhs.X, this->Y + rhs.Y)); 
		}	
		
		Point2D operator -(const Point2D& rhs)
		{
			return Point2D(Point2D(this->X - rhs.X, this->Y - rhs.Y)); 
		}	
		
		Point2D operator *(const Point2D& rhs)
		{
			return Point2D(Point2D(this->X * rhs.X, this->Y * rhs.Y)); 
		}	

		Point2D operator /(const Point2D& rhs)
		{
			return Point2D(Point2D(this->X / rhs.X, this->Y / rhs.Y)); 
		}	
	}; 

	struct Point2Df
	{	
		float X { 0.0f }, 
			Y { 0.0f }; 

		Point2Df()
		{
		}

		Point2Df(float x, float y) : X(x), Y(y)
		{
		}

		Point2Df operator +(const Point2Df& rhs)
		{
			return Point2Df(Point2Df(this->X + rhs.X, this->Y + rhs.Y)); 
		}	
		
		Point2Df operator -(const Point2Df& rhs)
		{
			return Point2Df(Point2Df(this->X - rhs.X, this->Y - rhs.Y)); 
		}	
		
		Point2Df operator *(const Point2Df& rhs)
		{
			return Point2Df(Point2Df(this->X * rhs.X, this->Y * rhs.Y)); 
		}	

		Point2Df operator /(const Point2Df& rhs)
		{
			return Point2Df(Point2Df(this->X / rhs.X, this->Y / rhs.Y)); 
		}	

		bool operator ==(const Point2Df rhs)
		{
			return (
				this->X == rhs.X &&
				this->Y == rhs.Y
			);	
		}

		bool operator !=(const Point2Df rhs)
		{
			return (
				this->X != rhs.X ||
				this->Y != rhs.Y
			);	
		}
		
		void operator =(const float rhs[2])
		{
			this->X = rhs[0];
			this->Y = rhs[1];
		}
	}; 

	struct Point3D	// Stores a 3D Point
	{
	public:
		int X, Y, Z; 

		Point3D() : X(0), Y(0), Z(0)
		{
		}
		
		Point3D(int x, int y, int z) : X(x), Y(y), Z(z)
		{
		}

		Point3D operator +(const Point3D& rhs)
		{
			return Point3D(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z);
		}
		
		Point3D operator -(const Point3D& rhs)
		{
			return Point3D(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
		}
		
		Point3D operator *(const Point3D& rhs)
		{
			return Point3D(this->X * rhs.X, this->Y * rhs.Y, this->Z * rhs.Z);
		}

		Point3D operator /(const Point3D& rhs)
		{
			return Point3D(this->X / rhs.X, this->Y / rhs.Y, this->Z / rhs.Z);
		}
	}; 

	struct Point3Df	// Stores a 3D Point
	{
	public:
		float X { 0.0f }, 
			Y { 0.0f }, 
			Z { 0.0f }; 

		Point3Df() : X(0.0f), Y(0.0f), Z(0.0f)
		{
		}

		Point3Df(float x, float y, float z) : X(x), Y(y), Z(z)
		{
		}

		Point3Df(float x, float y) : X(x), Y(y)
		{
		}

		Point3Df(float x) : X(x)
		{
		}

		Point3Df operator ==(const Point3D& rhs)
		{
			return Point3Df(this->X + rhs.X, this->Y + rhs.Y, this->Z + rhs.Z);
		}

		bool operator +(const Point3D& rhs)
		{	
			return (this->X == rhs.X && this->Y == rhs.Y && this->Z == rhs.Z);
		}
		
		Point3Df operator -(const Point3D& rhs)
		{
			return Point3Df(this->X - rhs.X, this->Y - rhs.Y, this->Z - rhs.Z);
		}
		
		Point3Df operator *(const Point3D& rhs)
		{
			return Point3Df(this->X * rhs.X, this->Y * rhs.Y, this->Z * rhs.Z);
		}

		Point3Df operator /(const Point3D& rhs)
		{
			return Point3Df(this->X / rhs.X, this->Y / rhs.Y, this->Z / rhs.Z);
		}

		void operator =(const float vertices[3])
		{
			this->X = vertices[0];
			this->Y = vertices[1];
			this->Z = vertices[2];
		}

		bool operator ==(const Point3Df& rhs)
		{
			return (
				rhs.X == this->X &&
				rhs.Y == this->Y &&
				rhs.Z == this->Z
			);	
		}

		bool operator !=(const Point3Df& rhs)
		{
			return (
				rhs.X != this->X ||
				rhs.Y != this->Y ||
				rhs.Z != this->Z
			);	
		}
	}; 

	struct Color	// Stores RGB values.
	{
		float R, G, B, A; 

		Color() : R(0), G(0), B(0), A(0)
		{		
		}
		
		Color(float r, float g, float b, float a) : R(r), G(g), B(b), A(0)
		{
		}	

		Color(float r, float g, float b) : R(r), G(g), B(b)
		{
		}
	}; 

	struct Vertex2D
	{
		Point2D Position; 

		Color VertexColor;

		Vertex2D() : Position(Point2D()), VertexColor(Color())
		{
		}

		Vertex2D(Point2D position) : Position(position), VertexColor(Color())
		{
		}

		Vertex2D(Point2D position, Color color) : Position(position), VertexColor(color)
		{
		}
	}; 

	struct Vertex2Df
	{
		Point2Df Position; 

		Color VertexColor;

		Vertex2Df() : Position(Point2Df()), VertexColor(Color())
		{
		}

		Vertex2Df(Point2Df position) : Position(position), VertexColor(Color())
		{
		}

		Vertex2Df(Point2Df position, Color color) : Position(position), VertexColor(color)
		{
		}
	}; 

	struct Vertex3D
	{
		Point3D Position;

		Color VertexColor;

		Vertex3D() : Position(Point3D()), VertexColor(Color())
		{
		}

		Vertex3D(Point3D position) : Position(position), VertexColor(Color())
		{
		}

		Vertex3D(Point3D position, Color color) : Position(position), VertexColor(color)
		{
		}
	}; 

	struct Vertex3Df
	{
	private:
		std::vector<float> SingleBuffer;	//	Temp for storing the linear buffer of the current instance.

	public:
		Point3Df Position; 

		Point3Df Normals;
		
		Point2Df TextureCoordinates;

		Color VertexColor;

		std::vector<float> GetSingleBuffer();	//	Returns a single buffer containing the vertex components

		Vertex3Df() : Position(Point3Df()), VertexColor(Color()), TextureCoordinates(Point2Df()), Normals(Point3Df())
		{
		}

		Vertex3Df(Point3Df position) : Position(position), VertexColor(Color()), TextureCoordinates(Point2Df())
		{
		}

		Vertex3Df(Point3Df position, Color color) : Position(position), VertexColor(color), TextureCoordinates(Point2Df())
		{
		}

		Vertex3Df(Point3Df position, Point2Df textureCoordinates) : Position(position), VertexColor(Color()), TextureCoordinates(textureCoordinates)
		{
		}

		Vertex3Df(Point3Df position, Point2Df textureCoordinates, Point3Df normals) : Position(position), VertexColor(Color()), TextureCoordinates(textureCoordinates), Normals(normals)
		{
		}

		Vertex3Df(Point3Df position, Color color, Point2Df textureCoordinates) : Position(position), VertexColor(color), TextureCoordinates(textureCoordinates)
		{
		}

		Vertex3Df(Point3Df position, Color color, Point2Df textureCoordinates, Point3Df normals) : Position(position), VertexColor(color), TextureCoordinates(textureCoordinates), Normals(normals)
		{
		}

		Vertex3Df(const float position[3], Color color, Point2Df textureCoordinates) : VertexColor(color), TextureCoordinates(textureCoordinates)
		{
			this->Position = position;
		}
	}; 
	// Contains data about a window.
}

// std::vector<GLEngine::Window*> AllocatedWindows = std::vector<GLEngine::Window*>();	// Execution Data
