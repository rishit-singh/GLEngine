#ifndef VERTEXARRAY_H

#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include "shader.h"
#include "tools.h"
#include "globjects.h"
#include "globals.h"
#include "matrix.h"

namespace GLEngine 
{
	enum ObjectType
	{
		VertexArray, 
		VertexBuffer,
		IndexBuffer
	};

	extern unsigned int GetByteSize(unsigned int);
	
	extern void SetBufferData(unsigned int, float*, unsigned int);

	template<typename T>
	bool SetBufferData(unsigned int bufferID, T* bufferData, int size, ObjectType objectType)	
	{	
		// if (!Bind(objectType, bufferID))	// 	checks the oject type and bind the buffer if the type is valid
		// 	return false;

		switch (objectType)
		{
			case VertexBuffer:
				glBindBuffer(GL_ARRAY_BUFFER, bufferID);
				glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), bufferData, GL_STATIC_DRAW);

				return true;

			case IndexBuffer:
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(T), bufferData, GL_STATIC_DRAW);

				return true;

			default:
				return false;
		}

		return false;		
	}

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
			glGenBuffers(1, &this->VertexBuffer);	//	Vertex buffer generation
			glGenBuffers(1, &this->IndexBuffer);	//	Index buffer generation

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

	class VertexArrayObject
	{
	private:
		void DeleteBufferObjects();	//	Deletes all the buffer objects

	public:		
		std::vector<VertexAttributeObject> VertexAttributes;	//	Vertex attributes for the buffer layout of the current VAO.
		
		std::vector<VertexBufferObject> VertexBufferObjects;	//	Stores all the VertexBufferObjects 	

		static GLenum* ObjectTypes; 	//	This is just a comment

		VertexBufferObject AddVertexBufferObject(VertexBufferObject);	//	Creates a VertexBufferObejct and adds it to the VAO
		
		bool AddVertexAttribute(VertexAttributeObject);	//	Creates a VertexBufferObejct and adds it to the VAO
		bool AddVertexAttribute(VertexAttributeObject, VertexBufferObject);	//	Creates a VertexBufferObejct and adds it to the VAO
		bool SetVertexAttributePointer();	//	Sets the Vertex Attribute pointer for the current vertex array instance.
		
		unsigned int GetBufferStride();	//	Generates the buffer stride for the Vertex Attribute pointer

		void Bind();	//	Binds the vertex array object
		void Bind(unsigned int);	//	Binds the vertex array object at the provided vertex array id
		void Unbind();	//	Unbinds the bound vertex array object 

		bool IsBound { false };	// Represents the binding status 

		unsigned int ID { 0 } ; // VAO ID wof the current instance

		VertexArrayObject(float* vertexDataArray, unsigned int vertexArraySize, unsigned int* indexArray, unsigned int indexArraySize) : VertexBufferObjects(std::vector<VertexBufferObject>()), VertexAttributes(std::vector<VertexAttributeObject>()), IsBound(false)
		{
			glGenVertexArrays(1, &this->ID);  // Generates the VAO			
			this->Bind();

			this->AddVertexBufferObject(VertexBufferObject(vertexDataArray, vertexArraySize, indexArray, indexArraySize));
			this->AddVertexAttribute(VertexAttributeObject(this->VertexAttributes.size() - 1, 3, GL_FLOAT, GL_FALSE), this->VertexBufferObjects.at(this->VertexAttributes.size())); 
		}	

		VertexArrayObject(std::vector<float> vertexDataArray, std::vector<unsigned int> indexArray) : VertexBufferObjects(std::vector<	VertexBufferObject>()), IsBound(false)
		{	
			glGenVertexArrays(1, &(this->ID));  // Generates the VAO
			this->Bind();

			this->AddVertexBufferObject(VertexBufferObject(vertexDataArray.data(), vertexDataArray.size(), indexArray.data(), indexArray.size()));
			this->AddVertexAttribute(VertexAttributeObject(this->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));

			// this->Unbind(); 
		}

		VertexArrayObject(VertexBufferObject vertexBufferObject)
		{
			glGenVertexArrays(1, &(this->ID));
			this->Bind();

			this->AddVertexBufferObject(vertexBufferObject);
			this->AddVertexAttribute(VertexAttributeObject(this->VertexAttributes.size(), 3, GL_FLOAT, GL_FALSE));

			this->SetVertexAttributePointer();
		}

		~VertexArrayObject()
		{
			glDeleteVertexArrays(1, &this->VertexAttributes.at(this->VertexAttributes.size() - 1).ID);

			this->DeleteBufferObjects();		
		}
	};
	
}

#endif
