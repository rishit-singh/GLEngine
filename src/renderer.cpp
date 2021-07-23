#include "renderer.h"

std::vector<GLEngine::GLEObject*> GLEngine::AllocatedGLEObjects = std::vector<GLEngine::GLEObject*>(); 

// void GLEngine::Renderer::Render(GLEngine::Mesh* mesh)
// {		
// 	glUseProgram(mesh->MeshShader->ShaderProgramID);

// 	glBindVertexArray(mesh->VAO->VertexAttributes.at(mesh->VAO->VertexAttributes.size() - 1).ID); 

// 	gl	DrawArrays(GL_TRIANGLES, 0, 3); 
// }

float colorValue = 1.0f;

float i = 0.1f;	// increament value	

void GLEngine::Renderer::Render(VertexArrayObject* vertexArrayObject, Shader* shader)
{
	shader->Enable();
	
	vertexArrayObject->Bind();
	vertexArrayObject->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer);

	glDrawElements(GL_TRIANGLES, vertexArrayObject->VertexBufferObjects.back().IndexArraySize, GL_UNSIGNED_INT, nullptr); 
}

void GLEngine::Renderer::Render(VertexArrayObject* vertexArrayObject, Shader* shader, Texture texture)
{
	texture.Bind();
	texture.SendToShader(shader);

	shader->Enable();

	vertexArrayObject->Bind();
	vertexArrayObject->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer);

	glDrawElements(GL_TRIANGLES, vertexArrayObject->VertexBufferObjects.back().IndexArraySize, GL_UNSIGNED_INT, nullptr); 

	texture.Unbind();
}

void GLEngine::Renderer::Render(std::vector<VertexArrayObject*> vertexArrayObjects, Shader* shader)
{
	int sizeTemp = vertexArrayObjects.size();

	if (!sizeTemp)
	{
		Debug->Log("No VertexArrayObject instances were provided.");

		return;
	}

	shader->Enable();

	for (int x = 0; x < sizeTemp; x++)
	{
		vertexArrayObjects.at(x)->Bind();
		vertexArrayObjects.at(x)->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer);

		glDrawElements(GL_TRIANGLES, vertexArrayObjects.at(x)->VertexBufferObjects.back().IndexArraySize, GL_UNSIGNED_INT, nullptr); 

		vertexArrayObjects.at(x)->Unbind();
	}
}

void GLEngine::Renderer::Render(std::vector<VertexArrayObject*> vertexArrayObjects, Shader* shader, std::vector<Texture> textures)
{
	int sizeTemp = vertexArrayObjects.size();

	if (!sizeTemp)
	{
		Debug->Log("No VertexArrayObject instances were provided.");

		return;
	}

	shader->Enable();

	for (int x = 0; x < sizeTemp; x++)
	{
		GLEngine::Renderer::Render(vertexArrayObjects.at(x), shader, textures.at(x));
	}
}


void GLEngine::Renderer::Render(GLEngine::GLEObject* object)
{
	object->ObjectMesh->MeshShader->Enable();
	// mesh->VAO.Bind(VertexArrayObject::VertexArray, 0);
	object->ObjectMesh->VertexArrayObjects.back()->Bind(); 	
	object->VAO->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer); 
	
	glDrawArrays(GL_TRIANGLES, 0, 3); // temporary rendering type
}

// bool GLEngine::Renderer::GLLoop(GLEngine::Window window, Mesh* mesh)
// {
// 	while (!glfwWindowShouldClose(window.GLWindow))
// 	{
// 		glClear(GL_COLOR_BUFFER_BIT);
// 		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);
		
// 		window.ProcessInput();
		
// 		GLEngine::Renderer::Render(mesh);	//	Renders the mesh  

// 		glfwSwapBuffers(window.GLWindow);
// 		glfwPollEvents();
// mesh	->VAO.Bind(VertexArrayObject::VertexArray, 0);
// 	} 

// 	return true;
// }

bool GLEngine::Renderer::GLLoop(GLEngine::Window window, GLEObject* object)
{
	while (!glfwWindowShouldClose(window.GLWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);
		
		window.ProcessInput();
		
		GLEngine::Renderer::Render(object);	//	Renders the mesh  

		glfwSwapBuffers(window.GLWindow);
		glfwPollEvents();
	}

	return true;
}


void GLEngine::Renderer::Render(GLEngine::Mesh mesh)
{
	int sizeTempY = mesh.VertexArrayObjects.size(), sizeTempX;

	mesh.MeshShader->Enable();


	for (int y = 0; y < sizeTempY; y++)
		for (int x = 0; x < mesh.VertexArrayObjects.at(y)->VertexBufferObjects.size(); x++)
		{	
			mesh.VertexArrayObjects.at(y)->Bind();
			mesh.VertexArrayObjects.at(y)->VertexBufferObjects.at(x).Bind(GLEngine::IndexBuffer);
			
			glDrawElements(GL_TRIANGLES, mesh.VertexArrayObjects.back()->VertexBufferObjects.back().IndexArraySize, GL_UNSIGNED_INT, nullptr);
		}
	// mesh.VertexArrayObjects.back()->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer);
}

// GLEngine::GLEObject::GLEObject() : ID(GLEngine::AllocatedGLEObjects.size()), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL)), MeshArray(std::vector<Mesh*>()), ObjectMesh(new Mesh())
// {
// }

// GLEngine::GLEObject::GLEObject(float* vertexMatrixArray) : ID(GLEngine::AllocatedGLEObjects.size()), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL)), MeshArray({ new Mesh(vertexMatrixArray, 9, Shader()) }), ObjectMesh(this->MeshArray.at(0))
// {
// }

// GLEngine::GLEObject::GLEObject(std::vector<Vertex3Df> vertexVectorArray) : ID(AllocatedGLEObjects.size()), VAO(new VertexArrayObject(nullptr, NULL, nullptr, NULL)), MeshArray({ new Mesh(vertexVectorArray) }), ObjectMesh(this->MeshArray.at(0))
// {
// 	this->VAO = new VertexArrayObject(this->ObjectMesh->VertexMatrixArray, 9, nullptr, NULL);
// }

// GLEngine::GLEObject::GLEObject(std::vector<Vertex3Df> vertexVectorArray, GLEngine::Shader shader) : ID(AllocatedGLEObjects.size()), MeshArray({ new Mesh(vertexVectorArray, shader) })//, ObjectMesh(this->MeshArray.at(0))
// {	
// 	this->ObjectMesh = this->MeshArray.at(0);
// 	// General::PrintVertexFloatArray(this->ObjectMesh->VertexMatrixArray, 9); s
// 	this->VAO = new VertexArrayObject(General::VertexVectorToFloatArray(this->ObjectMesh->VertexMatrixVector), this->ObjectMesh->VertexMatrixVector.size() * 3, nullptr, NULL); 	
// }


void GLEngine::Mesh::Update(unsigned int id)
{
	this->VertexArrayObjects.at(id)->SetVertexAttributePointer();
}

bool GLEngine::Mesh::AddVertexArrayObject(VertexArrayObject* vertexArrayObject)
{
	this->VertexArrayObjects.push_back(vertexArrayObject);

	this->VertexArrayObjects.back()->AddVertexAttribute(VertexAttributeObject(this->VertexArrayObjects.size(), 2, GL_FLOAT, GL_FALSE));
	this->VertexArrayObjects.back()->SetVertexAttributePointer();

	return true;
}
	
bool GLEngine::Mesh::AddBufferObject(VertexBufferObject vertexBufferObject, unsigned int index)
{
	if (!vertexBufferObject.IsValid())	//	Instance check
	{
		Debug->Log("VertexBufferObject instance provided.");

		return false;
	}
	
	if (index >= this->VertexArrayObjects.size())	//	Index check
	{
		Debug->Log("VAO index out of range.");

		return false;
	}

	this->VertexArrayObjects.back()->AddVertexBufferObject(vertexBufferObject);
	this->Update(index);

	return true;
}

void GLEngine::Mesh::DeleteVAOs()
{
	int sizeTemp = this->VertexArrayObjects.size();

	for (int x = 0; x < sizeTemp; x++)
		delete this->VertexArrayObjects.at(x);
}
