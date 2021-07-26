#include "window.h"
#include "tools.h"
#include "Debug.h"

// constructors to be forward declared.

std::vector<GLEngine::Window*> GLEngine::AllocatedWindows = std::vector<GLEngine::Window*>();

GLEngine::Window::Window() : /*ID(WindowStack.size()),*/ Title(DefaultStringValues[(int)DefaultStringType::WindowTitleStrings]), Resolution(DefaultResolutions[(int)DefaultResolutionType::DefaultWindow]), GLWindow(glfwCreateWindow(this->Resolution.X, this->Resolution.Y, this->Title, NULL, NULL)), BackgroundColor(Color(0, 0, 0, 0)), ID(AllocatedWindows.size())
{
	AllocatedWindows.push_back(this);

	if (this->GLWindow == nullptr)	//	Error check
	{
		std::cout << "\n Could not create the window. "; 
				
		glfwTerminate(); 
	}	
		
	glViewport(0, 0, this->Resolution.X, this->Resolution.Y); 
}

GLEngine::Window::Window(String title) : Title (title), Resolution(DefaultResolutions[(int)DefaultResolutionType::DefaultWindow]), GLWindow(glfwCreateWindow(this->Resolution.X, this->Resolution.Y, this->Title, NULL, NULL)), BackgroundColor(Color(0, 0, 0, 0)), ID(AllocatedWindows.size())
{
	AllocatedWindows.push_back(this);
}

GLEngine::Window::Window(String title, Color color) : Title (title), Resolution(DefaultResolutions[(int)DefaultResolutionType::DefaultWindow]), GLWindow(glfwCreateWindow(this->Resolution.X, this->Resolution.Y, this->Title, NULL, NULL)), BackgroundColor(Color((float)color.R, (float)color.G, (float)color.B, (float)color.A)), ID(AllocatedWindows.size())
{
	AllocatedWindows.push_back(this);

}

GLEngine::Window::Window(String title, Point2D resolution, Color color = Color(0, 0, 0, 0)) : Title(title), Resolution(resolution), GLWindow(glfwCreateWindow(resolution.X, resolution.Y, this->Title, NULL, NULL)), BackgroundColor(color), ID(AllocatedWindows.size())
{
	AllocatedWindows.push_back(this);

	// Debug->Log(color.R); 
	// Debug->Log(color.G); 
	// Debug->Log(color.B); 
	// Debug->Log(color.A); 	
}

GLEngine::Window::Window(Point2D resolution, Color color) : Title(DefaultStringValues[(int)DefaultStringType::WindowTitleStrings]), Resolution(resolution), GLWindow(glfwCreateWindow(resolution.X, resolution.Y, this->Title, NULL, NULL)), BackgroundColor(Color(0, 0, 0, 0)), ID(AllocatedWindows.size())
{
	AllocatedWindows.push_back(this);
}

GLEngine::Window::~Window()
{
	std::cout << "Window " << this->GLWindow << " deallocated. "; 
}

GLEngine::Window::Window(bool active) : /*ID(WindowStack.size()),*/ Title(DefaultStringValues[(int)DefaultStringType::WindowTitleStrings]), Resolution(DefaultResolutions[(int)DefaultResolutionType::DefaultWindow]), GLWindow(glfwCreateWindow(this->Resolution.X, this->Resolution.Y, this->Title, NULL, NULL)), BackgroundColor(Color(0, 0, 0, 0)), ID(AllocatedWindows.size())
{
	AllocatedWindows.push_back(this);

	if (this->GLWindow == nullptr)	//	Error check
	{
		std::cout << "\n Could not create the window. "; 

		glfwTerminate(); 				
	}	
		
	glViewport(0, 0, this->Resolution.X, this->Resolution.Y);

	if (active)
	{
		glfwMakeContextCurrent(this->GLWindow); 

		glfwSetFramebufferSizeCallback(this->GLWindow, Window::FrameBufferSizeCallBack); 
	}
}

void GLEngine::Window::FrameBufferSizeCallBack(GLFWwindow* window, int height, int width)
{
	glViewport(0, 0, height, width); 
}

void GLEngine::Window::Close()	
{
	glfwSetWindowShouldClose(this->GLWindow, true);
}


void GLEngine::Window::ProcessInput(GLEngine::Camera& camera)
{
	float CameraSpeed, CurrentFrame, LastFrame;

	GLEngine::SetDeltaTime(0.00005, &CameraSpeed, &CurrentFrame, &LastFrame);

	if (glfwGetKey(this->GLWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		this->Close(); 
		
	if (glfwGetKey(this->GLWindow, GLFW_KEY_J) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	if (glfwGetKey(this->GLWindow, GLFW_KEY_K) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (glfwGetKey(this->GLWindow, GLFW_KEY_W) == GLFW_PRESS)
		camera.Position += CameraSpeed * camera.Axis.Front;	

	if (glfwGetKey(this->GLWindow, GLFW_KEY_S) == GLFW_PRESS)
		camera.Position -= CameraSpeed * camera.Axis.Front;	

	if (glfwGetKey(this->GLWindow, GLFW_KEY_A) == GLFW_PRESS)
		camera.Position -= glm::normalize(glm::cross(camera.Axis.Front, camera.Axis.Up) * (CameraSpeed / 1000));	

	if (glfwGetKey(this->GLWindow, GLFW_KEY_D) == GLFW_PRESS)
		camera.Position += glm::normalize(glm::cross(camera.Axis.Front, camera.Axis.Up) * (CameraSpeed / 1000));	

	if (glfwGetKey(this->GLWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.Position.y += 0.1f;

	if (glfwGetKey(this->GLWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.Position.y -= 0.1f;

	if (glfwGetKey(this->GLWindow, GLFW_KEY_R) == GLFW_PRESS)
		camera = GLEngine::Camera(0, glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), GLEngine::CameraAxis(glm::vec3(0.0f, 0.0f, -0.05f), glm::vec3(0.0, 0.05f, 0.0f), 0.0f, -90.0f), 45.0f);
	
}

void GLEngine::SetDeltaTime(float initialSpeed, float* deltaTime, float* currentFrame, float* lastFrame)
{
    *deltaTime = (*currentFrame = (float)glfwGetTime()) - *lastFrame;
    *lastFrame = *currentFrame;   
}

void GLEngine::Window::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	float xOffSet, yOffSet;

	glm::vec3 frontTemp = glm::vec3();

	if (GLEngine::FirstMouse)
	{
		GLEngine::LastMousePosition.X = xPos;
		GLEngine::LastMousePosition.Y = yPos;

		GLEngine::FirstMouse = false;
	}

	xOffSet = xPos - GLEngine::LastMousePosition.X;
	yOffSet = GLEngine::LastMousePosition.Y - yPos;
	
	GLEngine::LastMousePosition.X = xPos;
	GLEngine::LastMousePosition.Y = yPos;

	xOffSet *= GLEngine::WindowCamera.Senstivity;
	yOffSet *= GLEngine::WindowCamera.Senstivity;

	GLEngine::WindowCamera.Axis.Yaw += xOffSet;
	GLEngine::WindowCamera.Axis.Pitch += yOffSet;

	GLEngine::General::SetRange(GLEngine::WindowCamera.Axis.Pitch, -89.0f, 89.0f);	//	Fixes the range
	
	frontTemp.x = glm::cos(glm::radians(GLEngine::WindowCamera.Axis.Yaw)) * glm::cos(glm::radians(GLEngine::WindowCamera.Axis.Pitch));
	frontTemp.y = glm::cos(glm::radians(GLEngine::WindowCamera.Axis.Pitch));
	frontTemp.z = glm::sin(glm::radians(GLEngine::WindowCamera.Axis.Yaw)) * glm::cos(glm::radians(GLEngine::WindowCamera.Axis.Pitch));

	GLEngine::WindowCamera.Axis.Front = glm::normalize(frontTemp);
}

void GLEngine::Window::ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset)
{
	GLEngine::WindowCamera.FOV -= yOffset;

	GLEngine::General::SetRange(GLEngine::WindowCamera.FOV, 1.0f, 45.0f);
}


