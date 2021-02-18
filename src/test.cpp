#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "globjects.h"
#include "glengine.h"
#include "window.h"
#include "shader.h"
#include "fileio.h"

using namespace GLEngine;  

int main()
{ 
	if (!SetupGLFW())	// Sets GLFW up
		return -1; 	

	SetWindowHints(4, 6); 

	Window window = Window("GLEngine App", Point2D(1280, 720), Color(0.2, 0.3, 0.3, 0.5));

	// glfwMakeContextCurrent(window.GLWindow); 

	SetCurrentContext(window.GLWindow); 
	glfwSetFramebufferSizeCallback(window.GLWindow, Window::FrameBufferSizeCallBack); 
	
	std::cout << "GLEW Status: " << SetupGLEW();     


	char* VertexShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/vertexshader.vert"),
	 *FragmentShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/fragmentshader.frag"); 
	
	Shader shader = Shader(VertexShaderString, FragmentShaderString, true); 

	VertexArrayObject* VAO = new VertexArrayObject({
			-0.2f, -0.2f, 0.0f, 
			-0.2f, 0.2f, 0.0f, 
			0.2f, 0.2f, 0.0f,
			0.2f, -0.2f, 0.0f 
		},

		{
			0, 1, 2,
			2, 3, 0 
		}
	);

	VAO->Unbind();

	unsigned int Location = glGetUniformLocation(shader.ShaderProgramID, "uFragmentColor");  

	float IncrementValue = 0.05f, ColorValue = 0.0f;	 

	while (!glfwWindowShouldClose(window.GLWindow))
	{
		window.ProcessInput(); 
	 	
		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);
		glClear(GL_COLOR_BUFFER_BIT);	

		Renderer::Render(VAO, &shader);

		glfwSwapBuffers(window.GLWindow);
		glfwPollEvents(); 
	}

	delete (&shader);

	glfwTerminate();

		
	return 0; 
}