#include "vertexarray.h"

GLenum* GLEngine::VertexArrayObject::ObjectTypes = new GLenum[3] {
	GL_VERTEX_ARRAY,
	GL_ARRAY_BUFFER, 
	GL_ELEMENT_ARRAY_BUFFER
}; 


unsigned int GLEngine::GetByteSize(unsigned int glType)
{
	switch (glType) 
	{
		case GL_UNSIGNED_INT:
			return 4; 
		
		case GL_FLOAT: 
			return 4;  

		case GL_UNSIGNED_BYTE: 
			return 1; 
		
		default:
			return 0; 
	}

	return 0; 
}

void GLEngine::VertexArrayObject::DeleteBufferObjects()	//	Deletes all the buffer objects
{
	int size = this->VertexBufferObjects.size(); 
	
	for (int x = 0; x < size; x++)
	{
		glDeleteBuffers(1, &this->VertexBufferObjects.at(x).VertexBuffer); 
		glDeleteBuffers(1, &this->VertexBufferObjects.at(x).IndexBuffer); 
	}
}

GLEngine::VertexBufferObject GLEngine::VertexArrayObject::AddVertexBufferObject(GLEngine::VertexBufferObject vertexBufferObject)
{
	if (!vertexBufferObject.IsValid())
	{
		Debug->Log("Invalid VertexBufferObject provided.");

		return VertexBufferObject(); 
	}

	this->VertexBufferObjects.push_back(vertexBufferObject);

	return this->VertexBufferObjects.back(); 
}

unsigned int GLEngine::VertexArrayObject::GetBufferStride()
{
	int bufferStride = 0, sizeTemp;

	sizeTemp = this->VertexAttributes.size(); 

	for (int x = 0; x < sizeTemp; x++)
		bufferStride += this->VertexAttributes.at(x).Stride * sizeof(float);

	return bufferStride;
}

bool GLEngine::VertexArrayObject::AddVertexAttribute(GLEngine::VertexAttributeObject vertexAttributeObject, GLEngine::VertexBufferObject vertexBufferObject)
{
	int sizeTemp = 0, offsetTemp = 0, strideTemp = 0, bufferStride = 0; 

	if (!vertexAttributeObject.IsValid())
	{
		Debug->Log("Invalid VertexBufferObject provided.");

		return false; 
	}

	this->VertexAttributes.push_back(vertexAttributeObject);

	sizeTemp = this->VertexAttributes.size();

	bufferStride = this->GetBufferStride();

	for (int x = 0; x < sizeTemp; x++)
	{
		glEnableVertexAttribArray(this->VertexAttributes.at(x).ID); 
		glVertexAttribPointer(this->VertexAttributes.at(x).ID, this->VertexAttributes.at(x).Stride, this->VertexAttributes.at(x).Type, this->VertexAttributes.at(x).Normalized, (strideTemp = (this->VertexAttributes.at(x).Stride * sizeof(float))), (void*)offsetTemp);//	this->VertexAttributes.back().Offset);

		offsetTemp += this->VertexAttributes.at(x).Stride * GetByteSize(GL_FLOAT);
	}
	
	return true; 
}

bool GLEngine::VertexArrayObject::AddVertexAttribute(GLEngine::VertexAttributeObject vertexAttributeObject)
{
	if (!vertexAttributeObject.IsValid())
	{
		Debug->Log("Invalid Vertex Attribute Object provided.");

		return false;
	}

	this->VertexAttributes.push_back(vertexAttributeObject);

	return true;
}

void GLEngine::VertexArrayObject::Bind()
{
	glBindVertexArray(this->ID); 
	this->IsBound = true;
}

void GLEngine::VertexArrayObject::Bind(unsigned int id)
{
	glBindVertexArray(id); 	
	this->IsBound = true;
}

bool GLEngine::VertexBufferObject::Bind(GLEngine::ObjectType objectType)
{
	switch (objectType)
	{
		case GLEngine::VertexBuffer:
			glBindBuffer(GL_ARRAY_BUFFER, this->VertexBuffer);
			
			return true; 

			break; 
		
		case GLEngine::IndexBuffer:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->IndexBuffer);

			return true; 

			break; 	

		default:
			return false;

			break; 
	}

	return false; 
}

bool GLEngine::VertexBufferObject::IsValid()
{
	return (
		this->VertexArray != nullptr &&
		this->IndexArray != nullptr &&
		this->VertexBuffer >= 0 &&
		this->IndexBuffer >= 0 &&
		this->VertexArraySize > 0 &&				
		this->IndexArraySize > 0 
	); 
}

bool GLEngine::VertexAttributeObject::IsValid()
{
	return (
		this->ID >= 0 &&
		this->Stride > 0
	); 
}

bool GLEngine::VertexBufferObject::UnBind(GLEngine::ObjectType objectType)
{
	switch (objectType)
	{
		case GLEngine::VertexBuffer:
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			
			return true; 

			break; 
		
		case GLEngine::IndexBuffer:
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			return true; 
			
			break; 	

		default:
			return false;

			break; 
	}

	return false; 
}

bool GLEngine::VertexArrayObject::SetVertexAttributePointer()
{
	int sizeTemp = 0, offsetTemp = 0, strideTemp, bufferStride; 

	sizeTemp = this->VertexAttributes.size();

	bufferStride = this->GetBufferStride();;

	for (int x = 0; x < sizeTemp; x++)
	{
		glEnableVertexAttribArray(this->VertexAttributes.at(x).ID); 
		glVertexAttribPointer(this->VertexAttributes.at(x).ID, this->VertexAttributes.at(x).Stride, this->VertexAttributes.at(x).Type, this->VertexAttributes.at(x).Normalized, bufferStride, (void*)offsetTemp);//	this->VertexAttributes.back().Offset);

		offsetTemp += this->VertexAttributes.at(x).Stride * GetByteSize(GL_FLOAT);
	}
	
	return true; 
}

void GLEngine::SetBufferData(unsigned int bufferID, float* vertexArray, unsigned int arraySize)
{	
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);	
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), vertexArray, GL_STATIC_DRAW);	//	Adds data to the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
}
