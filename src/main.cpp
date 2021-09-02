#include <iostream>	
#include <algorithm>
#include <math.h>
#include "glengine.h"
#include "fileio.h"
#include "objloader.h"

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

GLEngine::Camera GLEngine::WindowCamera = GLEngine::Camera(0, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), GLEngine::CameraAxis(glm::vec3(0.0f, 0.0f, -0.05f), glm::vec3(0.0, 0.05f, 0.0f), 0.0f, -90.0f), 45.0f);
// GLEngine::Camera GLEngine::WindowCamera = GLEngine::Camera(0, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), GLEngine::CameraAxis(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec3(0.0, 0.1f, 0.0f), 0.0f, -90.0f), 45.0f);

int main()
{	
	if (!SetupGLFW())
	{	
		Debug->Log("Failed to setup GLFW. "); 

		return -1; 
	}

	SetWindowHints(4, 6);
	
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	// Window window = Window("GLEngine App", Point2D(1280, 720), Color(0.2, 0.3, 0.3, 0.5)); 
	Window window = Window("GLEngine App", Point2D(1280, 720), Color(0.0f, 0.0f, 0.0f, 0.0f)); 

	SetCurrentContext(window); 

	// glfwSetInputMode(window.GLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	

	glfwSetFramebufferSizeCallback(window.GLWindow, Window::FrameBufferSizeCallBack); 
	// glfwSetCursorPosCallback(window.GLWindow, Window::MouseCallback);
	glfwSetScrollCallback(window.GLWindow, Window::ScrollCallBack);
	
	Debug->Log<bool>("GLEW Status: ", SetupGLEW()); 
	CreateDebugContext();

	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "Debug context check.");

	DefaultBlender.Enable(); 
	DefaultBlender.ApplyBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// GLEngine::Shader shader = GLEngine::Shader(FileIO::Read(GLEngine::DefaultPaths[(int)GLEngine::Shaders][(int)Shader::VertexShader]), 
	// 						FileIO::Read(GLEngine::DefaultPaths[(int)GLEngine::Shaders][(int)Shader::FragmentShader]), true);
	GLEngine::Shader shader = GLEngine::Shader(FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/vertexshader_ut.vert"), 
							FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/fragmentshader_ut.frag"), true);
	
	Texture texture = Texture(DefaultPaths[Textures][0]);

	Debug->Log("Shader compiled: ", shader.Verify());

	glEnable(GL_DEPTH_TEST);

	const int radius = 20.0f;
	
	std::vector<float> vb = OBJLoader::LoadOBJFile("/media/rishit/HDD0/src/repos/GLEngine/resources/torus.obj");

	GLEObject object = GLEObject(Mesh(new VertexArrayObject(vb, { 0 }), &shader, &texture));

	Debug->Log("vb.size()", vb.size());

	Transform transform = Transform();
	
	transform = Transform({ 0.1f, 0.0f, -2.0f });
	
	// transform.Rotate(45, { 0.0f, 1.0f, 0.0f });

	while (!glfwWindowShouldClose(window.GLWindow))
	{
		window.ProcessInput(GLEngine::WindowCamera);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);  
		glfwSwapInterval(1);
		
		DefaultWorldSpace.Projection = glm::perspective(glm::radians(WindowCamera.FOV), (float)1280 / (float)720, 0.1f, 100.0f);
		DefaultWorldSpace.View = WindowCamera.GetViewMatrix();
	
		object.ObjectMesh.MeshTransform.Rotate(1.0f * glfwGetTime(), { 1.0f, 1.0f, 0.0f });
	
		object.ObjectMesh.SetMVP();

		// glm::mat4 model = *object.ObjectMesh.MeshTransform.GetModelMatrix();

		// object.ObjectMesh.MeshShader->SetSquareMatrix<float>("model", glm::value_ptr(model), GL_FLOAT, 4);
		// object.ObjectMesh.MeshShader->SetSquareMatrix<float>("view", glm::value_ptr(DefaultWorldSpace.View), GL_FLOAT, 4);
		// object.ObjectMesh.MeshShader->SetSquareMatrix<float>("projection", glm::value_ptr(DefaultWorldSpace.Projection), GL_FLOAT, 4);
		
		Renderer::Render(object, 0);

		glfwSwapBuffers(window.GLWindow);
		glfwPollEvents();
	}

	object.Delete();
	
	return 0; 
}
