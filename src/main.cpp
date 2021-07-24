#include <iostream>	
#include <math.h>
#include "glengine.h"
#include "blending.h"
#include "fileio.h"

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
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

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

GLEngine::Camera camera = GLEngine::Camera(0, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(), glm::vec3(), GLEngine::CameraAxis(glm::vec3(0.0f, 0.0f, -0.05f), glm::vec3(0.0, 0.05f, 0.0f)));

void DrawRectangle(Vertex2Df dimenions, Vertex2Df location, Shader* shader)
{
	float xIncrement = dimenions.Position.X, yIncrement = dimenions.Position.Y;

	VertexArrayObject* VAO = new VertexArrayObject(
		{
			location.Position.X, location.Position.Y, 0.0f,		0.0f, 1.f,
			location.Position.X, location.Position.Y - yIncrement, 0.0f,		0.0f, 0.0f,
			location.Position.X + xIncrement, location.Position.Y - yIncrement, 0.0,	1.0f, 0.0f,
			location.Position.X + xIncrement, location.Position.Y, 0.0f,		1.0f, 1.0f
		},

		{
			0, 1, 2,
			2, 3, 0
		}
	);

	VAO->AddVertexAttribute(VertexAttributeObject(VAO->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));
	VAO->SetVertexAttributePointer();
	
	VAO->Unbind();

	Renderer::Render(VAO, shader);

	delete VAO;
}

void GenerateTileMap(Vertex2Df dimensions, Vertex2Df initialVertices, Shader* shader)
{
	if ((initialVertices.Position.Y > 1 || initialVertices.Position.Y < -1) || (initialVertices.Position.X > 1 || initialVertices.Position.X < -1))	//	Out of range check
	{
		Debug->Log("Vertices out of range.");

		return;
	}

	for (float y = initialVertices.Position.Y; y > -1.0f; y -= dimensions.Position.Y)
		for (float x = initialVertices.Position.X; x < 1.0f; x += dimensions.Position.X)
			DrawRectangle(Vertex2Df(Point2Df(dimensions.Position.X, dimensions.Position.Y)), Vertex2Df(Point2Df(x, y)), shader);
}


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
	glfwSetFramebufferSizeCallback(window.GLWindow, Window::FrameBufferSizeCallBack); 

	Debug->Log<bool>("GLEW Status: ", SetupGLEW()); 
	CreateDebugContext();

	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_MEDIUM, -1, "Debug context check.");

	Blender blender = Blender();

	blender.Enable(); 
	blender.ApplyBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	char* VertexShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/vertexshader.vert"), 
		*FragmentShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/fragmentshader.frag"); 

	GLEngine::Shader* shader = new GLEngine::Shader(VertexShaderString, FragmentShaderString, true	);

	Debug->Log("Shader compiled: ", shader->Verify());

	Texture texture = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/dirt.jpg"); 

	Mesh mesh = Mesh(
		{
			-0.3f, -0.3f, -0.3f,  0.0f, 0.0f,
			0.3f, -0.3f, -0.3f,  1.0f, 0.0f,
			0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
			0.3f,  0.3f, -0.3f,  1.0f, 1.0f,

			-0.3f,  0.3f, -0.3f,  0.0f, 1.0f,
			-0.3f, -0.3f, -0.3f,  0.0f, 0.0f,
			-0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
			0.3f, -0.3f,  0.3f,  1.0f, 0.0f,

			0.3f,  0.3f,  0.3f,  1.0f, 1.0f,
			0.3f,  0.3f,  0.3f,  1.0f, 1.0f,
			-0.3f,  0.3f,  0.3f,  0.0f, 1.0f,
			-0.3f, -0.3f,  0.3f,  0.0f, 0.0f,

			-0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
			-0.3f,  0.3f, -0.3f,  1.0f, 1.0f,
			-0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
			-0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
			
			-0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
			-0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
			0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
			0.3f,  0.3f, -0.3f,  1.0f, 1.0f,

			0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
			0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
			0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
			0.3f,  0.3f,  0.3f,  1.0f, 0.0f,

			-0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
			0.3f, -0.3f, -0.3f,  1.0f, 1.0f,
			0.3f, -0.3f,  0.3f,  1.0f, 0.0f,
			0.3f, -0.3f,  0.3f,  1.0f, 0.0f,

			-0.3f, -0.3f,  0.3f,  0.0f, 0.0f,
			-0.3f, -0.3f, -0.3f,  0.0f, 1.0f,
			-0.3f,  0.3f, -0.3f,  0.0f, 1.0f,
			0.3f,  0.3f, -0.3f,  1.0f, 1.0f,

			0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
			0.3f,  0.3f,  0.3f,  1.0f, 0.0f,
			-0.3f,  0.3f,  0.3f,  0.0f, 0.0f,
			-0.3f,  0.3f, -0.3f,  0.0f, 1.0f
		},	

		{
			0, 1, 2, 3, 
			4, 5, 6, 7, 
			8, 9, 10, 11, 
			12, 13, 14, 15, 
			16, 17, 18, 19, 
			20, 21, 22, 23, 
			24, 25, 26, 27, 
			28, 29, 30, 31, 
			32, 33, 34, 35
		}, 
		
	shader, &texture);

	glEnable(GL_DEPTH_TEST);

	const int radius = 20.0f;

	while (!glfwWindowShouldClose(window.GLWindow))
	{
		window.ProcessInput(camera);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);  
		glfwSwapInterval(1);
	
		MVPMatrixObject mvp = MVPMatrixObject(glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));

		mvp.View = camera.GetViewMatrix();
		
		for (float x = 0.0f; x < 10.0f; x += 1.0f)
		{
			mvp.Model = glm::rotate(mvp.Model, (float)glfwGetTime() * glm::radians(20 * (x + 1)), glm::vec3(0.5f, 1.0f, 0.0f));
			mvp.Projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);
			mvp.View = glm::translate(mvp.View, glm::vec3(x, 0.0f, -1.0f));
			
			unsigned int modelLocation = glGetUniformLocation(mesh.MeshShader->ShaderProgramID, "model"),
				viewLocation = glGetUniformLocation(mesh.MeshShader->ShaderProgramID, "view"),
				projectionLocation = glGetUniformLocation(mesh.MeshShader->ShaderProgramID, "projection");	

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mvp.Model));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &(mvp.View[0][0]));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(mvp.Projection));
			
			// float* model = glm::value_ptr(mvp.Model), 
			// *view = &(mvp.View[0][0]), 
			// *projection = glm::value_ptr(mvp.Projection);
			
			// mesh.MeshShader->SetMatrix4f("model", model);
			// mesh.MeshShader->SetMatrix4f("view", view);
			// mesh.MeshShader->SetMatrix4f("projection", projection);
			
			Renderer::Render(mesh);
		}

		glfwSwapBuffers(window.GLWindow);
		glfwPollEvents();
	}
	
	return 0; 
}
