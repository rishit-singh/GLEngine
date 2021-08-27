#include "renderer.h"

std::vector<GLEngine::GLEObject*> GLEngine::AllocatedGLEObjects = std::vector<GLEngine::GLEObject*>(); 

GLEngine::VertexBufferObject (*GLEngine::GLEObject::ObjectCreationFunctions[2])(GLEngine::Point3Df, Point3Df) {
	GLEngine::ShapeBufferGeneration::GenerateRectangle,
	GLEngine::ShapeBufferGeneration::GenerateCuboid
};

// GLEngine::Shader* GLEngine::DefaultShader* = new GLEngine::Shader(GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]), GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader]));

// void GLEngine::Renderer::GLEngine::Mesh* mesh)
// {		
// 	glUseProgram(mesh->Meshshader->Shader*ProgramID);

// 	glBindVertexArray(mesh->VAO->VertexAttributes.at(mesh->VAO->VertexAttributes.size() - 1).ID); 

// 	glDrawArrays(GL_TRIANGLES, 0, 3); 
// }

void GLEngine::Renderer::Render(VertexArrayObject* vertexArrayObject, Shader* shader)
{
	shader->Enable();
	
	vertexArrayObject->Bind();
	vertexArrayObject->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer);

	glDrawElements(GL_TRIANGLES, vertexArrayObject->VertexBufferObjects.back().IndexArraySize, GL_UNSIGNED_INT, nullptr); 
}

void GLEngine::Renderer::Render(VertexArrayObject* vertexArrayObject, Shader* shader, Texture* texture)
{
	texture->Bind();
	texture->SendToShader(shader);

	shader->Enable();

	vertexArrayObject->Bind();
	vertexArrayObject->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer);


	glDrawElements(GL_TRIANGLES, vertexArrayObject->VertexBufferObjects.back().IndexArraySize, GL_UNSIGNED_INT, nullptr); 
	texture->Unbind();
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

void GLEngine::Renderer::Render(std::vector<VertexArrayObject*> vertexArrayObjects, Shader* shader, std::vector<Texture*> textures)
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

void GLEngine::Renderer::Render(GLEngine::GLEObject object)
{
	int sizeTemp, sizeTemp1 = object.MeshArray.size();

	for (int y = 0; y < sizeTemp1; y++)
	{
		sizeTemp = object.MeshArray.at(y).VertexArrayObjects.size();

		for (int x = 0; x < sizeTemp; x++)
		{
			object.MeshArray.at(y).MeshShader->Enable();
			object.MeshArray.at(y).MeshTexture->Bind();

			object.MeshArray.at(y).VertexArrayObjects.at(x)->Bind(); 	
			object.MeshArray.at(y).VertexArrayObjects.at(x)->VertexBufferObjects.back().Bind(GLEngine::IndexBuffer); 

			glDrawElements(GL_TRIANGLES, object.MeshArray.at(y).VertexArrayObjects.at(x)->VertexBufferObjects.at(x).IndexArraySize, 
				GL_UNSIGNED_INT, nullptr);
		}	
	}
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

// bool GLEngine::Renderer::GLLoop(GLEngine::Window window, GLEObject* object)
// {
// 	while (!glfwWindowShouldClose(window.GLWindow))
// 	{
// 		glClear(GL_COLOR_BUFFER_BIT);
// 		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);
		
// 		window.ProcessInput();
		
// 		GLEngine::Renderer::Render(object);	//	Renders the mesh  

// 		glfwSwapBuffers(window.GLWindow);
// 		glfwPollEvents();
// 	}

// 	return true;
// }


void GLEngine::Renderer::RenderNonIndexed(GLEObject object)
{
	unsigned int meshArraySize = object.MeshArray.size(),
				vertexArraySize;

	
	for (int x = 0; x < meshArraySize; x++)
	{
		GLEngine::Mesh& mesh = object.MeshArray.at(x);

		vertexArraySize = mesh.VertexArrayObjects.size();
		
		mesh.MeshShader->Enable();
		mesh.MeshTexture->Bind();

		for (int y = 0; y < vertexArraySize; y++)
		{
			GLEngine::VertexArrayObject*& vao = mesh.VertexArrayObjects.at(y);		

			vao->VertexBufferObjects.back().Bind(GLEngine::VertexBuffer);

			glDrawArrays(GL_TRIANGLES, 0, vao->VertexBufferObjects.back().VertexArraySize);
		}
	} 
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

void GLEngine::Renderer::Render(GLEngine::GLEObject object, bool indexed)
{
	if (indexed)
		GLEngine::Renderer::Render(object);
	else 
		GLEngine::Renderer::RenderNonIndexed(object);
}

// GLEngine::GLEObject::GLEObject() : ID(GLEngine::AllocatedGLEObjects.size()), Type(GLEngine::GLEObject::Null), ObjectMesh(Mesh()), MeshArray(std::vector<Mesh>())
// {
// }

GLEngine::GLEObject::GLEObject(GLEngine::Mesh mesh) : ID(GLEngine::AllocatedGLEObjects.size()), Type(GLEngine::GLEObject::Unkown), ObjectMesh(mesh), MeshArray({ mesh }) 
{
	GLEngine::AllocatedGLEObjects.push_back(this);
}

GLEngine::GLEObject::GLEObject(GLEngine::Mesh mesh, GLEngine::GLEObject::GLEObjectType type) : ID(GLEngine::AllocatedGLEObjects.size()), Type(type), ObjectMesh(mesh), MeshArray({ mesh }) 
{
	GLEngine::AllocatedGLEObjects.push_back(this);
}

// GLEngine::GLEObject::GLEObject(std::vector<Vertex3Df> vertexVectorArray, GLEngine::GLEObject::GLEObjectType type) : ID(AllocatedGLEObjects	.size()), Type(type), ObjectMesh(Mesh()), MeshArray(std::vector<Mesh>())
// {
// 	std::vector<unsigned int> IndexArray = General::GetRangeArray<unsigned int>(vertexVectorArray.size() * 3);

// 	this->ObjectMesh = Mesh(GLEngine::General::VertexVectorToFloatArray(vertexVectorArray), vertexVectorArray.size() * 3, IndexArray.data(), (unsigned int)IndexArray.size(), new Shader(FileIO::Read(""), FileIO::Read("")), new Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/default.jpg"));

// 	this->MeshArray.push_back(this->ObjectMesh);
// }

// GLEngine::GLEObject::GLEObject(std::vector<Vertex3Df> vertexVectorArray, GLEngine::GLEObject::GLEObjectType type, GLEngine::Shader* shader) : ID(AllocatedGLEObjects.size())
// {	
// 	this->ObjectMesh = this->MeshArray.at(0);
// 	// General::PrintVertexFloatArray(this->ObjectMesh->VertexMatrixArray, 9); s
// }

GLEngine::GLEObject::GLEObject(std::vector<GLEngine::Vertex3Df> vertices) : MeshArray({ GLEngine::Mesh(vertices) })
{
	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
}

GLEngine::GLEObject::GLEObject(std::vector<GLEngine::VertexBufferObject> vertexBufferObjects, std::vector<GLEngine::Texture*> textures)
{
	int sizeTemp = vertexBufferObjects.size();

	for (int x = 0; x < sizeTemp; x++)
		this->MeshArray.push_back(Mesh(
			new VertexArrayObject(vertexBufferObjects.at(x)),
			new Shader(
				GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]),
				GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader]),
				true
			),
			textures.at(x)
		));

	this->ObjectMesh = this->MeshArray.front();
}


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

void GLEngine::GLEObject::CreateObject(std::vector<GLEngine::Vertex3Df> vertices, GLEngine::GLEObject::GLEObjectType type)
{
	Mesh mesh = Mesh();		//	mesh temp

	Shader* shader = new Shader(
		GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]),
		GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader]),
		true
	);

	Texture texture = Texture(GLEngine::DefaultPaths[1][0]);

	this->Type = (!General::InRange(type, 0, 3)) ? GLEngine::GLEObject::Unkown : type;
 
	mesh = (this->Type == GLEngine::GLEObject::Triangle) ? Mesh(new VertexArrayObject(GLEngine::ShapeBufferGeneration::GenerateTriangle(vertices)), shader, &texture) :
			Mesh(new VertexArrayObject(VertexBufferObject(General::VertexVectorToFloatVector(vertices), GLEngine::ShapeBufferGeneration::GenericIndexBuffers[type])), shader, &texture);		
	
	this->MeshArray.push_back(mesh);

	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
	
	return;
}

void GLEngine::GLEObject::CreateObject(GLEngine::Mesh mesh)
{
	this->MeshArray.push_back(mesh); 
	
	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
}

void GLEngine::GLEObject::CreateObject(GLEngine::Mesh mesh, GLEngine::GLEObject::GLEObjectType type)
{
	if (!GLEngine::General::InRange((int)type, -1, 5))
	{	
		this->Type = GLEngine::GLEObject::Unkown;

		return;
	}

	this->MeshArray.push_back(mesh);

	this->Type = type;	

	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
}


void GLEngine::GLEObject::CreateObject(std::vector<GLEngine::Vertex3Df> vertices)
{	
	GLEngine::Texture texture = GLEngine::Texture(GLEngine::DefaultPaths[GLEngine::Textures][0]);

	GLEngine::Shader* shader = new GLEngine::Shader(GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]), GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader]));

	this->MeshArray.push_back(GLEngine::Mesh(vertices, shader, texture));
	
	this->Type = GLEngine::GLEObject::Unkown;

	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
}

// void GLEngine::GLEObject::CreateObject(std::vector<GLEngine::Vertex3Df> vertices, GLEngine::GLEObject::GLEObjectType type)
// {	
// 	if (!GLEngine::General::InRange((int)type, -1, 5))
// 	{	
// 		this->Type = GLEngine::GLEObject::Unkown;

// 		return;
// 	}

// 	GLEngine::Texture texture = GLEngine::Texture(GLEngine::DefaultPaths[GLEngine::Textures][0]);

// 	this->MeshArray.push_back(GLEngine::Mesh(vertices, new GLEngine::Shader(GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]), GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader])), &texture));
	
// 	this->Type = type;

// 	if (!this->MeshArray.size())
// 		this->ObjectMesh = this->MeshArray.back();
// }

void GLEngine::GLEObject::CreateObject(GLEngine::Point3Df location, GLEngine::Point3Df dimensions, GLEngine::GLEObject::GLEObjectType type)
{
	if (!GLEngine::General::InRange((int)type, 1, 3))
	{	
		this->Type = GLEngine::GLEObject::Unkown;

		return;
	}

	GLEngine::Texture texture = GLEngine::Texture(GLEngine::DefaultPaths[GLEngine::Textures][0]);

	this->MeshArray.push_back(Mesh(new VertexArrayObject(GLEngine::GLEObject::ObjectCreationFunctions[(int)type - 1](location, dimensions)), new Shader(
			GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]),
			GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader])
		), &texture));

	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
}


void GLEngine::GLEObject::CreateObject(GLEngine::Point3Df location, GLEngine::Point3Df dimensions, GLEngine::Texture* texture, GLEngine::GLEObject::GLEObjectType type)
{
	if (!GLEngine::General::InRange((int)type, 1, 3))
	{	
		this->Type = GLEngine::GLEObject::Unkown;

		return;
	}

	this->MeshArray.push_back(Mesh(new VertexArrayObject(GLEngine::GLEObject::ObjectCreationFunctions[(int)type - 1](location, dimensions)), 
		new Shader(
			GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::VertexShader]),
			GLEngine::FileIO::Read(GLEngine::DefaultPaths[GLEngine::Shaders][GLEngine::Shader::FragmentShader])
		), texture));

	if (!this->MeshArray.size())
		this->ObjectMesh = this->MeshArray.back();
}

void GLEngine::GLEObject::Delete()
{
	int sizeTemp = this->ObjectMesh.VertexArrayObjects.size(), 
		sizeTemp1;	//	Sizetemp

	for (int x = 0; x < sizeTemp; x++)
	{
		sizeTemp1 = this->MeshArray.at(x).VertexArrayObjects.size();

		for (int y = 0; y < sizeTemp1; y++)
			delete this->MeshArray.at(x).VertexArrayObjects.at(y);
	}
}

GLEngine::VertexBufferObject GLEngine::ShapeBufferGeneration::GenerateTriangle(std::vector<GLEngine::Vertex3Df> vertices)
{
	return GLEngine::VertexBufferObject(GLEngine::General::ArrayToVector<float>(GLEngine::General::VertexVectorToFloatArray(vertices), vertices.size() * 3), { 0, 1, 2 });
}

GLEngine::VertexBufferObject GLEngine::ShapeBufferGeneration::GenerateRectangle(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)
{
	return GLEngine::VertexBufferObject(
		{
			location.X, location.Y, location.Z, 					0.0f, 1.0f,
			location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z, 				1.0f, 1.0f
		},
		
		GLEngine::ShapeBufferGeneration::GenericIndexBuffers[(int)GLEngine::GLEObject::Rectangle]	
	);
}

std::vector<float> GLEngine::ShapeBufferGeneration::GenerateRectangleVertices(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)	//	Generates a vertex buffer object for a rectangle with provided vertices at the provided vertices
{
	return {
		location.X, location.Y, location.Z, 					0.0f, 1.0f,
		location.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
		location.X + dimensions.X, location.Y, location.Z, 					0.0f, 1.0f
	};
}

GLEngine::VertexBufferObject GLEngine::ShapeBufferGeneration::GeneratePolygon(std::vector<GLEngine::Point3Df> triangle, std::vector<GLEngine::Point3Df> triangle1)
{
	VertexBufferObject vertexBufferObject = VertexBufferObject();

	if (triangle.at(0) != triangle1.at(0) && triangle.at(2) != triangle1.at(2))
		return vertexBufferObject;

	vertexBufferObject = VertexBufferObject({
		triangle.at(0).X, triangle.at(0).Y, triangle.at(0).Z,	0.0f, 1.0f,
		triangle.at(1).X, triangle.at(1).Y, triangle.at(1).Z,	0.0f, 0.0f,
		triangle.at(2).X, triangle.at(2).Y, triangle.at(2).Z,	1.0f, 0.0f,
		triangle1.at(1).X, triangle1.at(1).Y, triangle1.at(1).Z,	1.0f, 1.1f
	},
	GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Polygon]);

	return vertexBufferObject;
}

std::vector<GLEngine::Vertex3Df> GLEngine::ShapeBufferGeneration::GenerateRectangleVertexObjects(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)	//	Generates an array of Vertex3Df for a rectangle with provided vertices at the provided location
{
	return {
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z), GLEngine::Point2Df(1.0f, 1.0f))
	};
}

GLEngine::VertexBufferObject GLEngine::ShapeBufferGeneration::GenerateCuboid(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)
{
	return GLEngine::VertexBufferObject(
		{
			location.X, location.Y, location.Z, 					0.0f, 1.0f,	//	Front
			location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z, 					1.0f, 1.0f,

			location.X, location.Y, location.Z - dimensions.Z,		0.0f, 1.0f,	//	Back
			location.X, location.Y - dimensions.Y, location.Z - dimensions.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z,	1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z - dimensions.Z, 	1.0f, 1.0f,

			location.X, location.Y, location.Z - dimensions.Z, 		0.0f, 1.0f,	//	Top
			location.X, location.Y, location.Z,						0.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z,		1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z - dimensions.Z,	1.0f, 1.0f,

			location.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 		0.0f, 1.0f,
			location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,	1.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 	1.0f, 1.0f,

			location.X, location.Y, location.Z - dimensions.Z,		0.0f, 1.0f,
			location.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 		0.0f, 0.0f,	//	Left
			location.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
			location.X, location.Y, location.Z, 	1.0f, 1.0f,
			
			location.X + dimensions.X, location.Y, location.Z - dimensions.Z, 					0.0f, 1.0f,	//	Right
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z, 		1.0f, 1.0f			
		},
		
		{
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		}
	);
}

std::vector<float> GLEngine::ShapeBufferGeneration::GenerateCuboidVertices(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)
{
	return {
		location.X, location.Y, location.Z, 					0.0f, 1.0f,	//	Front
		location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
		location.X + dimensions.X, location.Y, location.Z, 					1.0f, 1.0f,

		location.X, location.Y, location.Z - dimensions.Z,		0.0f, 1.0f,	//	Back
		location.X, location.Y - dimensions.Y, location.Z - dimensions.Z,		0.0f, 0.0f,
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z,	1.0f, 0.0f,
		location.X + dimensions.X, location.Y, location.Z - dimensions.Z, 	1.0f, 1.0f,

		location.X, location.Y, location.Z - dimensions.Z, 		0.0f, 1.0f,	//	Top
		location.X, location.Y, location.Z,						0.0f, 0.0f,
		location.X + dimensions.X, location.Y, location.Z,		1.0f, 0.0f,
		location.X + dimensions.X, location.Y, location.Z - dimensions.Z,	1.0f, 1.0f,

		location.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 		0.0f, 1.0f,
		location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z,	1.0f, 0.0f,
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 	1.0f, 1.0f,

		location.X, location.Y, location.Z - dimensions.Z,		0.0f, 1.0f,
		location.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 		0.0f, 0.0f,	//	Left
		location.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
		location.X, location.Y, location.Z, 	1.0f, 1.0f,
		
		location.X + dimensions.X, location.Y, location.Z - dimensions.Z, 					0.0f, 1.0f,	//	Right
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z,		0.0f, 0.0f,
		location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
		location.X + dimensions.X, location.Y, location.Z, 		1.0f, 1.0f	
	};
}

std::vector<GLEngine::Vertex3Df> GLEngine::ShapeBufferGeneration::GenerateCuboidVertexObjects(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)
{
	return {
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z), GLEngine::Point2Df(1.0f, 1.0f)),
		
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z - dimensions.Z), GLEngine::Point2Df(1.0f, 1.0f)),
		
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z - dimensions.Z), GLEngine::Point2Df(1.0f, 1.0f)),
		
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z), GLEngine::Point2Df(1.0f, 1.0f)),

		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X, location.Y, location.Z), GLEngine::Point2Df(1.0f, 1.0f)),
		
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 1.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z), GLEngine::Point2Df(0.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y - dimensions.Y, location.Z), GLEngine::Point2Df(1.0f, 0.0f)),
		GLEngine::Vertex3Df(GLEngine::Point3Df(location.X + dimensions.X, location.Y, location.Z), GLEngine::Point2Df(1.0f, 1.0f))
	};
}

std::vector<GLEngine::VertexBufferObject> GLEngine::ShapeBufferGeneration::GenerateCuboidFaces(GLEngine::Point3Df location, GLEngine::Point3Df dimensions)
{
	return {
		VertexBufferObject({
			location.X, location.Y, location.Z, 					0.0f, 1.0f,	//	Front
			location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,	1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z, 		1.0f, 1.0f
		}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]),
	
		VertexBufferObject({
			location.X, location.Y, location.Z - dimensions.Z,		0.0f, 1.0f,	//	Back
			location.X, location.Y - dimensions.Y, location.Z - dimensions.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z,	1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z - dimensions.Z, 	1.0f, 1.0f
		}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]),

		VertexBufferObject({
			location.X, location.Y, location.Z - dimensions.Z, 		0.0f, 1.0f,	//	Top
			location.X, location.Y, location.Z,						0.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z,		1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z - dimensions.Z,	1.0f, 1.0f
		}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]),

		VertexBufferObject({
			location.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 		0.0f, 1.0f,
			location.X, location.Y - dimensions.Y, location.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,	1.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 	1.0f, 1.0f
		}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]),

		VertexBufferObject({
			location.X, location.Y, location.Z - dimensions.Z,		0.0f, 1.0f,
			location.X, location.Y - dimensions.Y, location.Z - dimensions.Z, 		0.0f, 0.0f,	//	Left
			location.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
			location.X, location.Y, location.Z, 	1.0f, 1.0f
		}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]),

		VertexBufferObject({
			location.X + dimensions.X, location.Y, location.Z - dimensions.Z, 					0.0f, 1.0f,	//	Right
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z - dimensions.Z,		0.0f, 0.0f,
			location.X + dimensions.X, location.Y - dimensions.Y, location.Z,		1.0f, 0.0f,
			location.X + dimensions.X, location.Y, location.Z, 		1.0f, 1.0f	
		}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]),
	};
}

