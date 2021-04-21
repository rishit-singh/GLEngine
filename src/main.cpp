#include <iostream>	
#include "glengine.h"
#include "texture.h"
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

	char* VertexShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/vertexshader.vert"), 
		*FragmentShaderString = FileIO::Read("/media/rishit/HDD0/src/repos/GLEngine/shaders/fragmentshader.frag"); 

	Shader* shader = new Shader(VertexShaderString, FragmentShaderString, true	);

	Debug->Log("Shader compiled: ", shader->Verify());

	VertexArrayObject* VAO = new VertexArrayObject({
			-0.3f, -0.55f, 0.0f, 0.0f, 0.0f, 
			-0.3f, 0.65f, 0.0f,	0.0f, 1.0f,
			0.3f, 0.65f, 0.0f, 1.0f, 1.0f,
			0.3f, -0.55f, 0.0f, 1.0f, 0.0f
		},		

		{
			0, 1, 2,
			2, 3, 0
		}
	);

	VAO->AddVertexAttribute(VertexAttributeObject(VAO->VertexAttributes.size(), 2, GL_FLOAT, GL_FALSE));	

	Texture texture = Texture("/media/rishit/HDD0/src/repos/GLEngine/resources/doomguy.png"); 
	
	texture.Bind(); 

	Debug->Log<bool>("texture.SendToShader(*shader)", texture.SendToShader(shader)); 

	// shader->SetUniformValue<int>("uTextureSlot", GL_INT, new int[1] { 0 }, 1); 

	// shader->SetUniformValue<int>("uTextureSlot", GL_INT, new int[1] { 0 }, 1); 

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
