#pragma once 

#define GLENGINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>
#include <vector>	
#include "shader.h"
#include "tools.h"
#include "window.h"
#include "globjects.h"
#include "globals.h"
#include "matrix.h"

using namespace DebugTools; 

namespace GLEngine
{
	enum ObjectType
	{
		VertexArray, 
		VertexBuffer,
		IndexBuffer
	};

	extern void SetWindowHints(unsigned int, unsigned int);	//	Sets up the OpenGL version hint
	extern void SetCurrentContext(Window);	//	Sets the provided instance of a GLFWwindow as the current context
	extern bool SetupGLEW();	//	Sets up the GLEW library
	extern bool SetupGLFW();	//	Sets up the GLFW library

	extern bool Bind(ObjectType, unsigned int);	//	Binds a specified VBO
	extern void SetBufferData(unsigned int);	// Sets main data.
	extern void SetBufferData(unsigned int, float*, unsigned int);	//	Sets the main data
	
	extern unsigned int GetByteSize(unsigned int);	//	Returns the byte size of the provided GL type


	template<typename T>
	bool SetBufferData(unsigned int bufferID, T* bufferData, int size, ObjectType objectType)	
	{	
		// if (!Bind(objectType, bufferID))	// 	checks the oject type and bind the buffer if the type is valid
		// 	return false;

		switch (objectType)
		{
			case GLEngine::VertexBuffer:
				glBindBuffer(GL_ARRAY_BUFFER, bufferID);
				glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), bufferData, GL_STATIC_DRAW);

				return true;

			case GLEngine::IndexBuffer:
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(T), bufferData, GL_STATIC_DRAW);

				return true;

			default:
				return false;
		}

		// glBindBuffer(VertexArrayObject::ObjectTypes[(int)objectType], bufferID);


		// Bind(VertexBuffer, 0); 

		return false;		
	}
	struct VertexAttributeObject	//	Stores Vertex  Attributes for the buffer layout
	{
		unsigned int ID, 
		Stride, 
		Type,
	 	Normalized,
		Offset;

		bool IsValid(); //	Checks if the current instance is valid.

		VertexAttributeObject() : ID(-1), Stride(0), Type(-1), Normalized(NULL), Offset(0)
		{		
		}

		VertexAttributeObject(int id, int stride, int type, unsigned int normalized) : ID(id), Stride(stride), Type(type),Normalized(normalized), Offset(0)
		{	
		}
	
		VertexAttributeObject(int id, int stride, int type, unsigned int normalized, unsigned int offset) : ID(id), Stride(stride), Type(type),Normalized(normalized), Offset(offset)
		{		
		}
	}; 

	struct VertexBufferObject
	{
		float* VertexArray;
		unsigned int* IndexArray;

		unsigned int VertexBuffer,
		IndexBuffer, 
		VertexArraySize,
		IndexArraySize;		

		bool IsValid();	//	Checks if an instance of the VertexBufferObjecet is valid for binding and rendering
		bool Bind(ObjectType);	//	Bind the specified buffer
		bool UnBind(ObjectType);	//	Unbinds the specified buffer

		VertexBufferObject() : IndexArray(nullptr), VertexBuffer(-1), IndexBuffer(-1)
		{
		}

		VertexBufferObject(float* vertexArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize) : VertexArray(vertexArray), IndexArray(indexArray), VertexArraySize(vertexArraySize), IndexArraySize(indexArraySize)
		{
			//	Buffer generation
			glGenBuffers(1, &this->VertexBuffer);
			glGenBuffers(1, &this->IndexBuffer);
 
			// Sets the data to the buffer
			SetBufferData<float>(this->VertexBuffer, this->VertexArray, this->VertexArraySize, ObjectType::VertexBuffer);
			SetBufferData<unsigned int>(this->IndexBuffer, this->IndexArray, this->IndexArraySize, ObjectType::IndexBuffer);			
		}

		VertexBufferObject(std::vector<float> vertexArrayVector, std::vector<unsigned int> indexArrayVector) : VertexArray(vertexArrayVector.data()), IndexArray(indexArrayVector.data()), VertexArraySize(vertexArrayVector.size()), IndexArraySize(indexArrayVector.size())
		{
			//	Buffer generation	
			glGenBuffers(1, &this->VertexBuffer);
			glGenBuffers(1, &this->IndexBuffer);
			
			// Sets the data to the buffer
			SetBufferData<float>(this->VertexBuffer, this->VertexArray, this->VertexArraySize, ObjectType::VertexBuffer);
			SetBufferData<unsigned int>(this->IndexBuffer, this->IndexArray, this->IndexArraySize, ObjectType::IndexBuffer);			
		}
	}; 

	class VertexArrayObject
	{
	private:
		void DeleteBufferObjects();	//	Deletes all the buffer objects

	public:		
		std::vector<VertexAttributeObject> VertexAttributes;	//	Vertex attributes for the buffer layout of the current VAO.
		
		std::vector<VertexBufferObject> VertexBufferObjects;	//	Stores all the VertexBufferObjects 	

		static GLenum* ObjectTypes; 

		VertexBufferObject AddVertexBufferObject(VertexBufferObject);	//	Creates a VertexBufferObejct and adds it to the VAO
		bool AddVertexAttribute(VertexAttributeObject);	//	Creates a VertexBufferObejct and adds it to the VAO
		bool AddVertexAttribute(VertexAttributeObject, VertexBufferObject);	//	Creates a VertexBufferObejct and adds it to the VAO
		void SetVertexAttributePointer(unsigned int);	//	Sets the Vertex Attribute pointer for the latest object
		void SetVertexAttributePointer(unsigned int, unsigned int);	//	Sets the Vertex Attribute pointer for the provided object id

		void Bind();	//	Binds the vertex array object
		void Bind(unsigned int);	//	Binds the vertex array object at the provided vertex array id
		void Unbind();	//	Unbinds the bound vertex array object 

		bool IsBound;	// Represents the binding status 

		unsigned int ID,	// VAO ID wof the current instance
		BufferLayoutStride;	//	Strides of all the vertex atrributes in the bufferlayout combined
	
		VertexArrayObject() : ID(-1), BufferLayoutStride(NULL), VertexBufferObjects(std::vector<VertexBufferObject>()), VertexAttributes(std::vector<VertexAttributeObject>()), IsBound(false)
		{
			// this->CreateBufferObject(); 
		}

		VertexArrayObject(int bufferCount) : ID(-1), BufferLayoutStride(NULL), VertexBufferObjects(std::vector<VertexBufferObject>()), VertexAttributes(std::vector<VertexAttributeObject>()), IsBound(false)
		{
			// this->CreateBufferObject(); 
		}

		VertexArrayObject(float* vertexDataArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize) : VertexBufferObjects(std::vector<VertexBufferObject>()), VertexAttributes(std::vector<VertexAttributeObject>()), IsBound(false)
		{ 
			// VertexBufferObject vertexBufferObject;
			this->AddVertexAttribute(VertexAttributeObject(this->VertexAttributes.size() - 1, 3, GL_FLOAT, GL_FALSE), this->VertexBufferObjects.at(this->VertexAttributes.size())); 

			glGenVertexArrays(1, &this->ID);  // Generates the VAO\			
			glBindVertexArray(this->ID);
			
			this->AddVertexBufferObject(VertexBufferObject(vertexDataArray, vertexArraySize, indexArray, indexArraySize));	//	To be replaced by a index buffer generation subroutine
			
			glEnableVertexAttribArray(this->VertexAttributes.back().ID);		
			glVertexAttribPointer(this->VertexAttributes.back().ID, this->BufferLayoutStride, this->VertexAttributes.back().Type, GL_FALSE, this->VertexAttributes.back().Stride * sizeof(float), (void*)this->VertexAttributes.back().Offset);

			// this->SetVertexAttributePointer(this->VertexAttributeObject.ID, this-0); // sets Vertex Attribute pointer to id 0.
		}	

		VertexArrayObject(std::vector<float> vertexDataArray, std::vector<unsigned int> indexArray) : VertexBufferObjects(std::vector<VertexBufferObject>()), IsBound(false)
		{	
			glGenVertexArrays(1, &(this->ID));  // Generates the VAO
			this->Bind();
		
			this->AddVertexBufferObject(VertexBufferObject(vertexDataArray.data(), vertexDataArray.size(), indexArray.data(), indexArray.size()));
			this->AddVertexAttribute(VertexAttributeObject(this->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE)); 
	
			Debug->Log<int>("this->VertexAttributes.size()", this->VertexAttributes.size()); 
		}

		~VertexArrayObject()
		{
			glDeleteVertexArrays(1, &this->VertexAttributes.at(this->VertexAttributes.size() - 1).ID);

			this->DeleteBufferObjects();		
		}
	};
	
	class Mesh 	//	3D/2D meshes
	{
	public:
		int MatrixSize;	//	VertexMatrix arra=y length

		VertexArrayObject VAO;	// Vertex Array Object 
		
		std::vector<Vertex3Df> VertexMatrixVector;	// Stores the triangle vertex matrix of a maesh

		Shader MeshShader;	// Shader object of the current mesh

		float* VertexMatrixArray;

		void SetVAO();	//	Sets the Vertex Array Object
		bool Enable();	//	Sets up the mesh by enabling the vertex array object and shaders	
		bool Update();	//	Recompiles all the shaders and resets the VAO

		Mesh() : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(new float[1000]), MatrixSize(0), MeshShader(Shader()) 
		{
		}
		
		Mesh(Vertex3Df* vertexMatrix, int size) : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(General::PointArrayToFloatArray(vertexMatrix, size)), MeshShader(Shader())
		{
		}

		Mesh(float* vertexMatrix, int size, Shader shader) : VertexMatrixVector(std::vector<Vertex3Df>()), VertexMatrixArray(vertexMatrix), MeshShader(shader)
		{
		}

		Mesh(std::vector<Vertex3Df> vertexMatrixVector) : VertexMatrixVector(vertexMatrixVector), VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(Shader())
		{
		}

		Mesh(std::vector<Vertex3Df>	vertexMatrixVector, Shader shader) : VertexMatrixVector(vertexMatrixVector),  VertexMatrixArray(General::VertexVectorToFloatArray(vertexMatrixVector)), MatrixSize(vertexMatrixVector.size() * 3), MeshShader(shader)
		{
			// if (vertexMatrixVector.size() > 0) 
				// this->SetVAO();
		}

		~Mesh()
		{
			delete &this->MeshShader;
			// delete &this->VAO;
		}
	};

	class GLEObject
	{
	public:
		enum GLEObjectType
		{
			Triangle,
			Polygon
		}; 
	
		unsigned int ID; 
		
		VertexArrayObject* VAO;	//	VertexArrayObject of the mesh
		Mesh* ObjectMesh;	//	Primary mesh / 0th element of the MeshArray

		std::vector<Mesh*> MeshArray; //	Stores all the meshes created for the current instance

		void CreateObject(float*); 	// Creates a new Mesh instance with the provided vertex array's elements as its vertices, and adds it to MeshArray
		void CreateObject(std::vector<Vertex2Df>);	// Creates a new Mesh instance with the provided Vertex3D array's vertice  as its vertices
		void CreateObject(std::vector<Vertex3Df>);	// Creates a new Mesh instance with the provided Vertex3Df array's elements as its vertices
		void CreateObject(Mesh);	// Adds the provieded mesh to MeshArray	

		GLEObject();
		GLEObject(float*); 
		GLEObject(std::vector<Vertex3Df>); 
		GLEObject(std::vector<Vertex3Df>, GLEObject::GLEObjectType); 
		GLEObject(std::vector<Vertex3Df>, Shader); 
		GLEObject(std::vector<Vertex3Df>, Shader, GLenum); 
		GLEObject(float*, int, Shader); 
	};

	class Renderer
	{
	public:	
		static bool IsNull(); // Null checks all required Mesh properties
		static bool GLLoop(Window, GLEObject*);	//	Runs the OpenGL loop for the provided GLEObject.
		static bool GLLoop(Window, Mesh*);	//	 Runs the OpenGL loop for the provided Mesh.
		static void Render(GLEObject*);	// Renders the provided GLEObject's mesh
		static void Render(Mesh*);	// Renders the provided Mesh
		static void Render(VertexArrayObject*, Shader*);	// Renders the psrovided Mesh
	};

	extern GLenum* GLObjectEnums; // Stores rendering type GLenums for different objects
	extern std::vector<GLEObject*> AllocatedGLEObjects;	//	Stores +all the instances of GLEObject created durintg the execution
}
