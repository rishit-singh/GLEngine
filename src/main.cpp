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

	Window window = Window("GLEngine App", Point2D(1280, 720), Color(0.2, 0.3, 0.3, 0.5)); 

	SetCurrentContext(window); 

	// glfwSetInputMode(window.GLWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	

	glfwSetFramebufferSizeCallback(window.GLWindow, Window::FrameBufferSizeCallBack); 
	// glfwSetCursorPosCallback(window.GLWindow, Window::MouseCallback);
	glfwSetScrollCallback(window.GLWindow, Window::ScrollCallBack);
	
	Debug->Log<bool>("GLEW Status: ", SetupGLEW()); 
	CreateDebugContext();

	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "Debug context check.");

	Blender blender = Blender();

	blender.Enable(); 
	blender.ApplyBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLEngine::Shader shader = GLEngine::Shader(FileIO::Read(GLEngine::DefaultPaths[(int)GLEngine::Shaders][(int)Shader::VertexShader]), 
							FileIO::Read(GLEngine::DefaultPaths[(int)GLEngine::Shaders][(int)Shader::FragmentShader]), true);
	
	Debug->Log("Shader compiled: ", shader.Verify());

	Texture texture = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/doomguy.png"), texture1 = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/cobblestone.png"),
	face = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/front.png"),
	top = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/top.png"),
	left = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/left.png"),
	right = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/right.png"),
	back = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/back.png"),
	torso = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/torso.png"),
	arm = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/arm.png"),
	leg = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/steve/leg.png");

	std::vector<float> CuboidVertices = ShapeBufferGeneration::GenerateCuboidVertices({ 0.0f, 1.0f, 0.0f }, { 0.15f, 0.15f, 0.15f });
	
	GLEObject object = GLEObject(Mesh(new VertexArrayObject(
		{
			CuboidVertices.at(0), CuboidVertices.at(1), CuboidVertices.at(2), CuboidVertices.at(3), CuboidVertices.at(4), 
			CuboidVertices.at(5), CuboidVertices.at(6), CuboidVertices.at(7), CuboidVertices.at(8), CuboidVertices.at(9), 
			CuboidVertices.at(10), CuboidVertices.at(11), CuboidVertices.at(12), CuboidVertices.at(13), CuboidVertices.at(14), 
			CuboidVertices.at(15), CuboidVertices.at(16), CuboidVertices.at(17), CuboidVertices.at(18), CuboidVertices.at(19), 
		},

		GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]
	), shader, &face));	//GLEObject(Mesh(new VertexArrayObject(ShapeBufferGeneration::GenerateCuboid(Point3Df(0.0f, 0.0f, 0.0f), Point3Df(10.0f, 1.0f, 10.0f))), shader, &texture));

	object.CreateObject(Mesh(new VertexArrayObject({
		CuboidVertices.at(20), CuboidVertices.at(21), CuboidVertices.at(22), CuboidVertices.at(23), CuboidVertices.at(24), 
		CuboidVertices.at(25), CuboidVertices.at(26), CuboidVertices.at(27), CuboidVertices.at(28), CuboidVertices.at(29), 
		CuboidVertices.at(30), CuboidVertices.at(31), CuboidVertices.at(32), CuboidVertices.at(33), CuboidVertices.at(34), 
		CuboidVertices.at(35), CuboidVertices.at(36), CuboidVertices.at(37), CuboidVertices.at(38), CuboidVertices.at(39), 		
	}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]), shader, &back), GLEngine::GLEObject::Rectangle);

	object.CreateObject(Mesh(new VertexArrayObject({
		CuboidVertices.at(40), CuboidVertices.at(41), CuboidVertices.at(42), CuboidVertices.at(43), CuboidVertices.at(44), 
		CuboidVertices.at(45), CuboidVertices.at(46), CuboidVertices.at(47), CuboidVertices.at(48), CuboidVertices.at(49), 
		CuboidVertices.at(50), CuboidVertices.at(51), CuboidVertices.at(52), CuboidVertices.at(53), CuboidVertices.at(54), 
		CuboidVertices.at(55), CuboidVertices.at(56), CuboidVertices.at(57), CuboidVertices.at(58), CuboidVertices.at(59), 		
	}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]), shader, &top), GLEngine::GLEObject::Rectangle);

	object.CreateObject(Mesh(new VertexArrayObject({
		CuboidVertices.at(60), CuboidVertices.at(61), CuboidVertices.at(62), CuboidVertices.at(63), CuboidVertices.at(64), 
		CuboidVertices.at(65), CuboidVertices.at(66), CuboidVertices.at(67), CuboidVertices.at(68), CuboidVertices.at(69), 
		CuboidVertices.at(70), CuboidVertices.at(71), CuboidVertices.at(72), CuboidVertices.at(73), CuboidVertices.at(74), 
		CuboidVertices.at(75), CuboidVertices.at(76), CuboidVertices.at(77), CuboidVertices.at(78), CuboidVertices.at(79), 		
	}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]), shader, &top), GLEngine::GLEObject::Rectangle);

	object.CreateObject(Mesh(new VertexArrayObject({
		CuboidVertices.at(80), CuboidVertices.at(81), CuboidVertices.at(82), CuboidVertices.at(83), CuboidVertices.at(84), 
		CuboidVertices.at(85), CuboidVertices.at(86), CuboidVertices.at(87), CuboidVertices.at(88), CuboidVertices.at(89), 
		CuboidVertices.at(90), CuboidVertices.at(91), CuboidVertices.at(92), CuboidVertices.at(93), CuboidVertices.at(94), 
		CuboidVertices.at(95), CuboidVertices.at(96), CuboidVertices.at(97), CuboidVertices.at(98), CuboidVertices.at(99), 		
	}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]), shader, &left), GLEngine::GLEObject::Rectangle);

	object.CreateObject(Mesh(new VertexArrayObject({
		CuboidVertices.at(100), CuboidVertices.at(101), CuboidVertices.at(102), CuboidVertices.at(103), CuboidVertices.at(104), 
		CuboidVertices.at(105), CuboidVertices.at(106), CuboidVertices.at(107), CuboidVertices.at(108), CuboidVertices.at(109), 
		CuboidVertices.at(110), CuboidVertices.at(111), CuboidVertices.at(112), CuboidVertices.at(113), CuboidVertices.at(114), 
		CuboidVertices.at(115), CuboidVertices.at(116), CuboidVertices.at(117), CuboidVertices.at(118), CuboidVertices.at(119), 		
	}, GLEngine::ShapeBufferGeneration::GenericIndexBuffers[GLEngine::GLEObject::Rectangle]), shader, &right), GLEngine::GLEObject::Rectangle);

	object.CreateObject({
		-0.04f, 0.85f, 0.0f
	}, {
		0.23f, 0.3f, 0.1f
	}, &torso, GLEObject::Cuboid);

	object.CreateObject({
		-0.12f, 0.85f, 0.0f
	}, {
		0.08f, 0.28f, 0.1f
	}, &arm, GLEObject::Cuboid);

	object.CreateObject({
		0.19f, 0.85f, 0.0f
	}, {
		0.08f, 0.28f, 0.1f
	}, &arm, GLEObject::Cuboid);

	object.CreateObject({
		-0.04f, 0.55f, 0.0f
	}, {
		0.12f, 0.3f, 0.1f
	}, &leg, GLEObject::Cuboid);

	object.CreateObject({
		0.07f, 0.55f, 0.0f
	}, {
		0.12f, 0.3f, 0.1f
	}, &leg, GLEObject::Cuboid);

	glEnable(GL_DEPTH_TEST);

	const int radius = 20.0f;

	while (!glfwWindowShouldClose(window.GLWindow))
	{
		window.ProcessInput(GLEngine::WindowCamera);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);  
		glfwSwapInterval(1);
		
		object.ObjectMesh.MVPMatrix = new MVPMatrixObject(glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));
		object.ObjectMesh.MVPMatrix->Projection = glm::perspective(glm::radians(WindowCamera.FOV), (float)1280 / (float)720, 0.1f, 100.0f);
		object.ObjectMesh.MVPMatrix->View = GLEngine::WindowCamera.GetViewMatrix();
		
		object.ObjectMesh.MVPMatrix->Model = glm::translate(object.ObjectMesh.MVPMatrix->Model, glm::vec3(0.1f, 0.0f, -2.0f));
		// object.ObjectMesh.MVPMatrix->Model = glm::rotate(object.ObjectMesh.MVPMatrix->Model, glm::radians(20), glm::vec3(1.0f, 0.3f, 0.5f));

		object.ObjectMesh.MeshShader.SetSquareMatrix<float>("model", glm::value_ptr(object.ObjectMesh.MVPMatrix->Model), GL_FLOAT, 4);
		object.ObjectMesh.MeshShader.SetSquareMatrix<float>("view", glm::value_ptr(object.ObjectMesh.MVPMatrix->View), GL_FLOAT, 4);
		object.ObjectMesh.MeshShader.SetSquareMatrix<float>("projection", glm::value_ptr(object.ObjectMesh.MVPMatrix->Projection), GL_FLOAT, 4);
			
		Renderer::Render(object);

		glfwSwapBuffers(window.GLWindow);
		glfwPollEvents();	
	}

	object.Delete();
	
	return 0; 
}
