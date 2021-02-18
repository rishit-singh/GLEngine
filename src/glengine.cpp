#include "glengine.h"
#include "exceptions.h"

GLenum* GLEngine::GLObjectEnums = new GLenum[1] {
	GL_TRIANGLES
}; 

GLenum* GLEngine::VertexArrayObject::ObjectTypes = new GLenum[3] {
	GL_VERTEX_ARRAY,
	GL_ARRAY_BUFFER, 
	GL_ELEMENT_ARRAY_BUFFER
}; 

std::vector<GLEngine::GLEObject*> GLEngine::AllocatedGLEObjects = std::vector<GLEngine::GLEObject*>(); 

void GLEngine::VertexArrayObject::DeleteBufferObjects()	//	Deletes all the buffer objects
{
	int size = this->VertexBufferObjects.size(); 
	
	for (int x = 0; x < size; x++)
	{
		glDeleteBuffers(1, &this->VertexBufferObjects.at(x).VertexBuffer); 
		glDeleteBuffers(1, &this->VertexBufferObjects.at(x).IndexBuffer); 
	}
}
bool GLEngine::VertexArrayObject::AddVertexBufferObject(GLEngine::VertexBufferObject vertexBufferObject)
{
	if (!vertexBufferObject.IsValid())
	{
		Debug->Log("Invalid VertexBufferObject provided.");

		return  false; 
	}

	this->VertexBufferObjects.push_back(vertexBufferObject);

	return true; 
}

bool GLEngine::VertexArrayObject::AddVertexAttribute(GLEngine::VertexAttributeObject vertexAttributeObject)
{
	if (!vertexAttributeObject.IsValid())
	{
		Debug->Log("Invalid VertexBufferObject provided.");

		return  false; 
	}

	this->VertexAttributes.push_back(vertexAttributeObject);

	return true; 
}

void GLEngine::SetWindowHints(unsigned int major, unsigned int minor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
} 

void GLEngine::SetCurrentContext(GLFWwindow* window)
{	
	glfwMakeContextCurrent(window);
}

bool GLEngine::SetupGLEW()
{
	glewExperimental = GL_TRUE; 

	try
	{
		if (glewInit() != GLEW_OK)
			throw new LibraryInitializationError("GLEW");
	}
	catch (const LibraryInitializationError& e)
	{	
		std::cout << "\nGLEW  initialization error..";

		return false; 
	}
	
	return true;
}

bool GLEngine::SetupGLFW()
{
	if (!glfwInit())
	{
		std::cout << "\nGLFW initialization error.";

		return false; 
	}

	return true;
}

void GLEngine::VertexArrayObject::Bind()
{
	glBindVertexArray(this->ID); 
}

void GLEngine::VertexArrayObject::Bind(unsigned int id)
{
	glBindVertexArray(id); 	
}

void GLEngine::VertexArrayObject::Unbind()
{
	glBindVertexArray(0); 
}

void GLEngine::Renderer::Render(GLEngine::Mesh* mesh)
{		
	glUseProgram(mesh->MeshShader.ShaderProgramID);

	glBindVertexArray(mesh->VAO.VertexAttributes.at(mesh->VAO.VertexAttributes.size() - 1).ID); 

	glDrawArrays(GL_TRIANGLES, 0, 3); 
}

void GLEngine::Renderer::Render(VertexArrayObject* vertexArrayObject, Shader* shader)
{
	shader->Enable();

	vertexArrayObject->Bind(); 

	vertexArrayObject->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer); 

	glDrawElements(GL_TRIANGLES, 256, GL_UNSIGNED_INT, nullptr); 
}

void GLEngine::Renderer::Render(GLEngine::GLEObject* object)
{
	glUseProgram(object->ObjectMesh->MeshShader.ShaderProgramID);
	// mesh->VAO.Bind(VertexArrayObject::VertexArray, 0);s
	glBindVertexArray(object->ObjectMesh->VAO.VertexAttributes.at(object->ObjectMesh->VAO.VertexAttributes.size() - 1).ID); 
	object->VAO->VertexBufferObjects.at(object->VAO->VertexBufferObjects.size() - 1).Bind(GLEngine::IndexBuffer); 
	
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
// 	// mesh->VAO.Bind(VertexArrayObject::VertexArray, 0);
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



// void GLEngine::Mesh::SetVAO()
// {
// 	this->VAO = GLEngine::VertexArrayObject(this->VertexMatrixArray, this->MatrixSize); 
// }

// void GLEngine::VertexArrayObject::CreateBufferObject(GLEngine::VertexArrayObject::ObjectType objectType)
// {
// 	unsigned int BufferObject; 

// 	glGenBuffers(1, &BufferObject);

// 	switch (objectType)
// 	{
// 		case GLEngine::VertexArrayObject::VertexBuffer:
// 			this->VertexBufferObjects.push_back(VertexBufferObject());

// 			this->VertexBufferObjects.data();

// 			break;

// 		case GLEngine::VertexArrayObject::IndexBuffer:
// 			this->IndexBufferObjects.push_back(BufferObject);

// 			break;
// 	}
// }

// void GLEngine::VertexArrayObject::CreateBufferObject()
// {
// 	unsigned int BufferObject;	// buffer object to be created

// 	glGenBuffers(1, &BufferObject); 

// 	this->VertexBufferObjects.push_back(BufferObject);
// }

bool GLEngine::Bind(GLEngine::ObjectType objectType, unsigned int id)
{

	try
	{
		switch (objectType)
		{
			case VertexBuffer: 
				glBindBuffer(GL_ARRAY_BUFFER, id);
				
				break;

			case VertexArray: 
				glBindVertexArray(id); 

				break;

			case IndexBuffer:
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);

				break; 

			default:
				throw new GLEInvalidTypeException("VertexObject"); 		

				break; 
		}
	}
	catch (const GLEInvalidTypeException& e)
	{
		Debug->Log(e.Message);	

		return false; 
	}
																													
	return true; 
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

template<typename T>
bool GLEngine::SetBufferData(unsigned int bufferID, T* bufferData, int size, ObjectType objectType)	
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

void GLEngine::SetBufferData(unsigned int bufferID, float* vertexArray, unsigned int arraySize)
{	
	// Bind(VertexBuffer, bufferID); 	//	Binds the provided vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);	
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), vertexArray, GL_STATIC_DRAW);	//	Adds data to the buffer
	// glBindBuffer(GL_ARRAY_BUFFER, 0);	

	// Bind(VertexBuffer, 0); 
}

void GLEngine::VertexArrayObject::SetVertexAttributePointer(unsigned int id, unsigned int size)	// todo: make argumentes more specific.
{
	// this->Bind(VertexArrayObject::VertexArray, id); 
	glVertexAttribPointer(id, size, GL_FLOAT, GL_FALSE, size * sizeof(float), (void*)0);
	glEnableVertexAttribArray(id);
}	

GLEngine::GLEObject::GLEObject() : ID(GLEngine::AllocatedGLEObjects.size()), VAO(new VertexArrayObject()), MeshArray(std::vector<Mesh*>()), ObjectMesh(new Mesh())
{
}

GLEngine::GLEObject::GLEObject(float* vertexMatrixArray) : ID(GLEngine::AllocatedGLEObjects.size()), VAO(new VertexArrayObject()), MeshArray({ new Mesh(vertexMatrixArray, 9, Shader()) }), ObjectMesh(this->MeshArray.at(0))
{
}

GLEngine::GLEObject::GLEObject(std::vector<Vertex3Df> vertexVectorArray) : ID(AllocatedGLEObjects.size()), VAO(new VertexArrayObject()), MeshArray({ new Mesh(vertexVectorArray) }), ObjectMesh(this->MeshArray.at(0))
{
	this->VAO = new VertexArrayObject(this->ObjectMesh->VertexMatrixArray, 9, nullptr, NULL);
}

GLEngine::GLEObject::GLEObject(std::vector<Vertex3Df> vertexVectorArray, GLEngine::Shader shader) : ID(AllocatedGLEObjects.size()), MeshArray({ new Mesh(vertexVectorArray, shader) })//, ObjectMesh(this->MeshArray.at(0))
{	
	this->ObjectMesh = this->MeshArray.at(0);
	// General::PrintVertexFloatArray(this->ObjectMesh->VertexMatrixArray, 9); s
	this->VAO = new VertexArrayObject(General::VertexVectorToFloatArray(this->ObjectMesh->VertexMatrixVector), this->ObjectMesh->VertexMatrixVector.size() * 3, nullptr, NULL); 	
}

GLEngine::GLEObject::GLEObject(float* vertexVectorArray, int arraySize, GLEngine::Shader shader) : ID(AllocatedGLEObjects.size()), MeshArray({ new Mesh(vertexVectorArray, arraySize, shader) }), ObjectMesh(this->MeshArray.at(0))
{
}

bool GLEngine::Mesh::Enable()
{
	Bind(GLEngine::VertexBuffer, this->VAO.VertexBufferObjects.at(this->VAO.VertexBufferObjects.size() - 1).VertexBuffer);	//	binds the last element FOR NOW
	glUseProgram(this->MeshShader.ShaderProgramID); 

	return true;	
} 

bool GLEngine::Mesh::Update()
{
	this->VAO = VertexArrayObject((this->VertexMatrixArray = General::VertexVectorToFloatArray(this->VertexMatrixVector)), (this->MatrixSize = this->VertexMatrixVector.size() * 3), nullptr, NULL); 	
}

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
