#include <iostream>	
#include "glengine.h"
#include "texture.h"
#include "blending.h"
#include "fileio.h"

using namespace GLEngine; 

int main()
{
	if (!SetupGLFW())
	{	
		Debug->Log("Failed to setup GLFW. "); 

		return -1; 
	} 

	SetWindowHints(4, 6);

	Window window = Window("GLEngine App", Point2D(1280, 720), Color(0.2, 0.3, 0.3, 0.5)); 

	SetCurrentContext(window); 
	glfwSetFramebufferSizeCallback(window.GLWindow, Window::FrameBufferSizeCallBack); 

	Debug->Log<bool>("GLEW Status: ", SetupGLEW()); 

	Blender blender = Blender();

	blender.Enable(); 
	blender.ApplyBlend(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	char* VertexShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/vertexshader.vert"), 
		*FragmentShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/fragmentshader.frag"); 

	GLEngine::Shader* shader = new GLEngine::Shader(VertexShaderString, FragmentShaderString, true	);

	Debug->Log("Shader compiled: ", shader->Verify());

	VertexArrayObject* VAO = new VertexArrayObject({
			-0.2f, -0.65f, 0.0f, 0.0f, 0.0f,
			-0.2f, 0.75f, 0.0f,	0.0f, 1.0f,	
			0.2f, 0.75f, 0.0f, 1.0f, 1.0f,
			0.2f, -0.65f, 0.0f, 1.0f, 0.0f,
		},		

		{
			0, 1, 2,
			2, 3, 0
		}
	);

	VAO->AddVertexAttribute(VertexAttributeObject(VAO->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));	

	Texture texture = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/doomguy_facing_straight.png"); 
	
	texture.Bind(); 

	Debug->Log<bool>("texture.SendToShader(*shader)", texture.SendToShader(shader)); 

	VAO->Unbind();

	while (!glfwWindowShouldClose(window.GLWindow))
	{
		window.ProcessInput();

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(window.BackgroundColor.R, window.BackgroundColor.G, window.BackgroundColor.B, window.BackgroundColor.A);  

		glfwSwapInterval(1); 

		Renderer::Render(VAO, shader); 		

		glfwSwapBuffers(window.GLWindow); 
		glfwPollEvents();
	}

	return 0; 
}
