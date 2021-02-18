#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

typedef char* String;   

enum ShaderType
{
	Vertex, 
	Fragment
}; 

enum StatusType
{
	Compile, 
	Link
}; 		  


// static bool InRange(int val, int lower, int upper)
// {
// 	return (val >= lower || val <= upper);
// }

static void GLClearError()
{
	while (!glGetError());
}

static void GLCheckErrors() 
{
	GLenum errorEnum; 

	while (!(errorEnum = glGetError()))
		std::cout << "\n[OpenGL Error]: " << errorEnum << '\n'; 
}

static FILE* GetFilePointer(String path, String mode)
{
	FILE* FilePointer; 

	if ((FilePointer = fopen(path, mode)) == nullptr)
		std::cout << '\n' << path << " was not found. \n";
	
	return FilePointer; 
}

static bool Write(String data, String path)	//	Writes data to the provided file
{
	FILE* FilePointer = GetFilePointer(path, "w");

	fprintf(FilePointer, "%s", data);

	return (FilePointer != nullptr);	//	successful representation(true) when the file pointer is not nullpte
}

static String Read(String path)
{
	FILE* FilePointer = GetFilePointer(path, "r"); 
	
	long BufferSize;	// File conten byte size

	String Buffer; //	buffer to store read bytes in		

	//	Getting buffer size
	fseek(FilePointer, 0, SEEK_END);

	BufferSize = ftell(FilePointer);

	fseek(FilePointer, 0, SEEK_SET);

	Buffer = (String)malloc(BufferSize + 1);	//	Allocating the buffer
	
	//	Reading bytes
	fread(Buffer, 1, BufferSize, FilePointer);
	fclose(FilePointer); 

	Buffer[BufferSize] = 0;	//	Sets the EOF

	return Buffer;
}

static void ProcessWindow(GLFWwindow* window)
{
	bool toggle = false; 

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) ==  GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);	

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0	, width, height);
}

static bool CheckErrors(unsigned int shaderID, StatusType status)
{	
	int operationStatus = NULL;

	String ErrorLog = new char[512];

	switch (status)
	{
		case Compile:
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &operationStatus);

			if (!operationStatus)	
			{
				glGetShaderInfoLog(shaderID, 512, NULL, ErrorLog);

				std::cout << "Shader compile/link error: " << ErrorLog	<< '\n';
	
				return true;
			}		

			break;

		case Link:
			glGetProgramiv(shaderID, GL_LINK_STATUS, &operationStatus);

			if (!operationStatus)	
			{
				glGetProgramInfoLog(shaderID, 512, NULL, ErrorLog);

				std::cout << "\nShader compile/link error: " << ErrorLog << '\n';
			
				return true; 	
			}

			break;
		
			default:	
				std::cout << "\nInvalid status type provieded\n"; 

				return true;

				break;
	}
	return false; 
}

GLenum* ShaderTypes = new GLenum[2] {
	GL_VERTEX_SHADER, 
	GL_FRAGMENT_SHADER
}; 

static unsigned int CompileShader(String shader, ShaderType shaderType) 
{
	if ((int)shaderType < 0 || (int)shaderType > 1)
	{
		std::cout << "Invalid shadertype enum provided. "; 
		
		return -1; 
	}

	unsigned int ShaderID = glCreateShader(ShaderTypes[(int)shaderType]);

	glShaderSource(ShaderID, 1, &shader, NULL);
	glCompileShader(ShaderID); 

	CheckErrors(ShaderID, Compile);

	return ShaderID;
} 

static unsigned int LinkShaders(unsigned int shader, unsigned int shader1)
{
	unsigned int LinkedShaderProgram  = glCreateProgram();

	glAttachShader(LinkedShaderProgram, shader);
	glAttachShader(LinkedShaderProgram, shader1);

	glLinkProgram(LinkedShaderProgram);

	glValidateProgram(LinkedShaderProgram); 
	CheckErrors(LinkedShaderProgram, Link); 

	glDeleteShader(shader);
	glDeleteShader(shader1);

	return LinkedShaderProgram;
}	

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW.";

		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifndef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(640, 480, "Triangle", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Couldn't create a GLFW window." << '\n';

		glfwTerminate();

		return -1;  
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(3); 

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW. \n";

		return -1; 
	}

	String* ShaderStrings = new String[2] {
		Read("/home/rishit/src/repos/GLEngine/test/shader.vert"),
		Read("/home/rishit/src/repos/GLEngine/test/shader.frag")
	}; 

	std::cout << "\n VertexShader:\n" << ShaderStrings[0]
			<< "\n\n FragmentShader:\n" << ShaderStrings[1];

	unsigned int ShaderProgram = LinkShaders(CompileShader(ShaderStrings[0], Vertex), CompileShader(ShaderStrings[1], Fragment));

	float* Vertices = new float[36] {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f 
	}; 

	unsigned int* Indices = new unsigned int[6] {
		0, 1, 2,
		2, 3, 0
	}; 
 
	unsigned int VertexArray, VertexBuffer, IndexBuffer;
	
	glGenVertexArrays(1, &VertexArray);
	glGenBuffers(1, &VertexBuffer);

	glBindVertexArray(VertexArray); 
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, 9 * 2 * sizeof(float), Vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &IndexBuffer); 	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer); 	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), Indices, GL_STATIC_DRAW); 	

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

	glBindVertexArray(0);

	float* Vertices1 = new float[9] {
		-0.5f, 0.5f, 0.0f, 
		0.0f, 0.5f, 0.0f, 
		0.0f, 1.0f, 0.0f
	}; 

	unsigned int VertexArray1, VertexBuffer1, 
		IndexBuffer1;

	glGenVertexArrays(1, &VertexArray1); 
	glBindVertexArray(VertexArray1); 

	glGenBuffers(1, &VertexBuffer1); 
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer1); 

	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), Vertices1, GL_STATIC_DRAW); 
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 

	unsigned int* Indices1 = new unsigned int[3] {
		0, 1, 2
	}; 

	glGenBuffers(1, &IndexBuffer1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer1);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(unsigned int), Vertices1, GL_STATIC_DRAW); 

	glUseProgram(ShaderProgram);

	float* ColorBuffer = new float[4] {
		1.0f, 0.4f, 0.8f, 1.0f
	};

	int Location; 

	glUniform4f((Location = glGetUniformLocation(ShaderProgram, "Color")), ColorBuffer[0], ColorBuffer[1], ColorBuffer[2], ColorBuffer[3]); 

	float Change = 0.05f;

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	
	glUseProgram(0); 

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		ProcessWindow(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(ShaderProgram);
		glUniform4f(Location, ColorBuffer[0] += Change, ColorBuffer[1], ColorBuffer[2], ColorBuffer[3]); 
	
		// glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer); 
		glBindVertexArray(VertexArray); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer); 

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(VertexArray1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer1);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr); 

		if (ColorBuffer[0] < 0.0f)	
			Change = 0.05f;

		if (ColorBuffer[0] > 1.0f)
			Change = -0.05f;

		// glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);

		glfwPollEvents(); 
	}
	
	glDeleteProgram(ShaderProgram);

	free(Vertices);
	free(Indices);

	glfwTerminate(); 

	return 0; 
}