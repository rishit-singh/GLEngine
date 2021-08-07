#include <iostream>	
#include <math.h>
#include "glengine.h"
#include "blending.h"
#include "fileio.h"
#include <algorithm>


using namespace GLEngine; 

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam);	//	OpenGL Debug call
void CreateDebugContext();

void DrawRectangle(Vertex2Df, Vertex2Df, Shader*);
void GenerateTileMap(Vertex2Df, Vertex2Df, Shader*);
void GenerateTileMap(Vertex2Df, Vertex2Df, Shader*, Point2Df);
void GenerateTileMap(Point2D, VertexArrayObject, Shader*);

void glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " <<  message << std::endl;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;
	
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

void CreateDebugContext()
{
	int flags; 

	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);


	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 

		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	} 
}

// GLEngine::Camera GLEngine::WindowCamera = GLEngine::Camera(0, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), GLEngine::CameraAxis(glm::vec3(0.0f, 0.0f, -0.05f), glm::vec3(0.0, 0.05f, 0.0f), 0.0f, -90.0f), 45.0f);
GLEngine::Camera GLEngine::WindowCamera = GLEngine::Camera(0, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), GLEngine::CameraAxis(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec3(0.0, 0.1f, 0.0f), 0.0f, -90.0f), 45.0f);

int main()
{	
	if (!SetupGLFW())
	{	
		Debug->Log("Failed to setup GLFW. "); 

		return -1; 
	}

	SetWindowHints(4, 6);
	
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	Window window = Window("GLEngine App", Point2D(1280, 720), Color(0.2, 0.3, 0.3, 0.5)); 

	SetCurrentContext(window); 

	glfwSetInputMode(window.GLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	

	glfwSetFramebufferSizeCallback(window.GLWindow, Window::FrameBufferSizeCallBack); 
	glfwSetCursorPosCallback(window.GLWindow, Window::MouseCallback);
	glfwSetScrollCallback(window.GLWindow, Window::ScrollCallBack);
	
	Debug->Log<bool>("GLEW Status: ", SetupGLEW()); 
	CreateDebugContext();

	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "Debug context check.");

	Blender blender = Blender();

	blender.Enable(); 
	blender.ApplyBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLEngine::Shader* shader = new GLEngine::Shader(FileIO::Read(GLEngine::DefaultPaths[(int)GLEngine::Shaders][(int)Shader::VertexShader]), 
							FileIO::Read(GLEngine::DefaultPaths[(int)GLEngine::Shaders][(int)Shader::FragmentShader]), true);
	Debug->Log("Shader compiled: ", shader->Verify());

	Texture texture = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/doomguy.png"); 
	Texture texture1 = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/cobblestone.png"); 


	GLEObject object = GLEObject(Mesh(new VertexArrayObject(ShapeBufferGeneration::GenerateCuboid(Point3Df(0.0f, 0.0f, 0.0f), Point3Df(10.0f, 1.0f, 10.0f))), shader, &texture));

	object.CreateObject({ 0.7f, 0.7f, 0.0f }, { 0.5f, 1.0f, 0.0f }, &texture1, GLEObject::Rectangle);
	// object.CreateObject(Point3Df(0.7f, 0.7f, 0.0f), Point3Df(0.5f, 0.5f, 1.0f), GLEObject::Cuboid);
 
	// object.CreateObject(Mesh(new VertexArrayObject(ShapeBufferGeneration::GenerateCuboid(Point3Df(1.5f, 0.5f, 0.0f), Point3Df(0.5f, 0.5f, 0.5f))), shader, &texture1));
 
	// object.CreateObject(Mesh(new VertexArrayObject(ShapeBufferGeneration::GenerateCuboid(Point3Df(2.5f, 0.5f, 0.0f), Point3Df(0.5f, 0.5f, 0.5f))), shader, &texture1));

	General::PrintArray<float>(object.ObjectMesh.VertexArrayObjects.back()->VertexBufferObjects.back().VertexArray,
						object.ObjectMesh.VertexArrayObjects.back()->VertexBufferObjects.back().VertexArraySize);


	glEnable(GL_DEPTH_TEST);

	const int radius = 20.0f;

	float x = 0.1f;

	while (!glfwWindowShouldClose(window.GLWindow))
	{
		window.ProcessInput(GLEngine::WindowCamera);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);  
		glfwSwapInterval(1);
		
		object.ObjectMesh.MVPMatrix = new MVPMatrixObject(glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));
		object.ObjectMesh.MVPMatrix->Projection = glm::perspective(glm::radians(WindowCamera.FOV), (float)1280 / (float)720, 0.1f, 100.0f);
		object.ObjectMesh.MVPMatrix->View = GLEngine::WindowCamera.GetViewMatrix();
		
		// for (float x = 0.0f; x < 5.0f; x += 1.0f)
		// {
		// 	object.ObjectMesh.MVPMatrix->Model = glm::translate(object.ObjectMesh.MVPMatrix->Model, glm::vec3(x, 0.0f, -2.0f));
		// 	object.ObjectMesh.MVPMatrix->Model = glm::rotate(object.ObjectMesh.MVPMatrix->Model, (float)glfwGetTime() * glm::radians(20 * x), glm::vec3(1.0f, 0.3f, 0.5f));

		// 	object.ObjectMesh.MeshShader->SetSquareMatrix<float>("model", glm::value_ptr(object.ObjectMesh.MVPMatrix->Model), GL_FLOAT, 4);
		// 	object.ObjectMesh.MeshShader->SetSquareMatrix<float>("view", glm::value_ptr(object.ObjectMesh.MVPMatrix->View), GL_FLOAT, 4);
		// 	object.ObjectMesh.MeshShader->SetSquareMatrix<float>("projection", glm::value_ptr(object.ObjectMesh.MVPMatrix->Projection), GL_FLOAT, 4);
			
		// 	Renderer::Render(object);
		// }
		
		object.ObjectMesh.MVPMatrix->Model = glm::translate(object.ObjectMesh.MVPMatrix->Model, glm::vec3(x, 0.0f, -2.0f));
		// object.ObjectMesh.MVPMatrix->Model = glm::rotate(object.ObjectMesh.MVPMatrix->Model, glm::radians(20), glm::vec3(1.0f, 0.3f, 0.5f));

		object.ObjectMesh.MeshShader->SetSquareMatrix<float>("model", glm::value_ptr(object.ObjectMesh.MVPMatrix->Model), GL_FLOAT, 4);
		object.ObjectMesh.MeshShader->SetSquareMatrix<float>("view", glm::value_ptr(object.ObjectMesh.MVPMatrix->View), GL_FLOAT, 4);
		object.ObjectMesh.MeshShader->SetSquareMatrix<float>("projection", glm::value_ptr(object.ObjectMesh.MVPMatrix->Projection), GL_FLOAT, 4);
			
		Renderer::Render(object);

		glfwSwapBuffers(window.GLWindow);
		glfwPollEvents();	
	}
	
	return 0; 
}
