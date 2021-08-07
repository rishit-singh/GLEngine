#include <stdlib.h>
#include "shader.h"
#include "exceptions.h"

GLenum* GLEngine::ShaderTypeGLenums = new GLenum[2] {
	GL_VERTEX_SHADER, 
	GL_FRAGMENT_SHADER
};

bool GLEngine::Shader::Enable()
{
	if (this->Verify())	
	{	
		glUseProgram(this->ShaderProgramID);
		
		return true; 
	}

	return false; 
}

bool GLEngine::Shader::Disable()
{
	glUseProgram(0); 

	return false; 
}

bool GLEngine::Shader::Verify()
{
	bool Validity = true;	
	 
	for (int x = 0; x < 2; x++)	
		if (this->ShaderIDs[x] <= 0 || this->ShaderProgramStrings.at(x) == nullptr)	
			Validity = !Validity;

	return Validity;
}

int GLEngine::Shader::GetUniformLocation(char* uniformName, unsigned int shaderProgramID)
{
	int location; 

	if (this->UniformHash[uniformName] != NULL)	//	Saves some GPU clocks
		return this->UniformHash[uniformName];

	location = glGetUniformLocation(shaderProgramID, uniformName); 


	if (location == -1)
	{
		char* message = strcat(strcat(strcat((char*)malloc(sizeof(char) * 256), "Uniform "), uniformName), " not found.");

		Debug->Log(message);

		free(message);
	}

	return location;
}

void GLEngine::Shader::DeleteShaders()
{
	for (int x = 0; x < 2; x++)
		glDeleteShader(this->ShaderIDs[x]); 
}

unsigned int* GLEngine::Shader::Compile()
{
	try
	{	
		if (!this->Verify())	//	Shader validation
			throw new GLEngine::GLEInvalidShaderException();
	}
	catch(GLEngine::GLEInvalidShaderException* e)
	{
		e->LogExceptionMessage();		

		return nullptr;
	}

	for (int x = 0; x < 2; x++)
	{
		glShaderSource(this->ShaderIDs[x], 1, &this->ShaderProgramStrings.at(x), NULL);
		glCompileShader(this->ShaderIDs[x]);


		this->CheckErrors(ShaderIDs[x], GL_COMPILE_STATUS);
	}

	return (this->ShaderIDs);
}

unsigned int GLEngine::Shader::Link()
{	
	unsigned int LinkedProgram = glCreateProgram();

	for (int x= 0; x < 2; x++)
		glAttachShader(LinkedProgram, this->ShaderIDs[x]);
	
	glLinkProgram(LinkedProgram);

	this->CheckErrors(LinkedProgram, GL_LINK_STATUS);	//	checks for linking errors

	return (this->ShaderProgramID = LinkedProgram); 
}

void GLEngine::Shader::CheckErrors(unsigned int program, GLenum glStatus)
{	
	int success;	//	complie/link status

	char* GLCompilerLog = new char[512]; 	

	switch (glStatus)
	{
		case GL_COMPILE_STATUS:	
			glGetShaderiv(program, glStatus, &success);

			try
	 		{
				if (!success) 
				{
					glGetShaderInfoLog(ShaderProgramID, 512, NULL, GLCompilerLog);

					throw new GLEngine::GLEInvalidShaderException(GLCompilerLog); 
				}
			}
			catch (GLEngine::GLEInvalidShaderException* e)
			{	
				e->LogExceptionMessage(); 
				
				std::cout << "\n GLSL compiler log: " << GLCompilerLog; 
			}

			break;

		case GL_LINK_STATUS:
			glGetProgramiv(program, glStatus, &success);

			try
			{
				if (!success) 
				{
					glGetProgramInfoLog(ShaderProgramID, 512, NULL, GLCompilerLog);
				
					throw new GLEngine::GLEInvalidShaderException(GLCompilerLog);
				}
			}
			catch (const GLEngine::GLEInvalidShaderException& e)
			{
				std::cout << "\n GLSL linker log: " << GLCompilerLog; 
			}

			break;
	}
}

bool GLEngine::Shader::SetMatrix4f(const char* uniformName, glm::mat4& matrix)
{
	// GLEngine::Debug->Log(uniformName);

	int location = glGetUniformLocation(this->ShaderProgramID, uniformName);

	// GLEngine::Debug->Log("Location retrieved");

	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));

	// GLEngine::Debug->Log("Uniform set")
}

