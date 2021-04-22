#pragma once

#define SHADER_H

#include "globjects.h"
#include "globals.h"
#include <unordered_map>
#include <any> 

namespace GLEngine
{
	extern GLenum* ShaderTypeGLenums; 	// Stores GLenums of the respective Shader types

	class Shader
	{
	private:
		int GetUniformLocation(char*, unsigned int); 

		void DeleteShaders();	//	Deletes the created shaders

	public:
		enum ShaderType
		{
			VertexShader, 
			FragmentShader
		};
		
		unsigned int ShaderProgramID; // Stores the final compiled shader program ID

		unsigned int* ShaderIDs;	// Array to store the program IDs of the		

		std::vector<String> ShaderProgramStrings; // Array to store them shader program string

		std::unordered_map<String, int> UniformHash; 	//	Hashtable storing all the shader uniform locations 

		bool Enable();	//	Enables the shader for rendering
		bool Disable();	//	Disables the enabled shader 
		bool Verify();	// Checks if the current instance of the Shader is eligible to get compiled and linked to the final shader program
		void CheckErrors(unsigned int, GLenum); // Checks for compiling and linking errors

		unsigned int* Compile();	//	Compiles all the shaders at once if eligible
		unsigned int Link();	//	Links the compiled shaders into one shader program						

		// template<typename T>
		// bool SetUniformValue(char*, unsigned int, T*, int); 	//	Gets the specified uniform location from the current linked shader program

		template<typename T>
		bool SetUniformValue(char* uniformName, unsigned int glType, T* data, int size)
		{
			switch (glType)
			{
				case GL_FLOAT:
					switch (size)
					{
						case 1:
							glUniform1f(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0]); 

							delete data;

							return true; 

							break; 

						case 2:
							glUniform2f(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0], data[1]); 

							delete data;

							return true; 

							break; 

						case 3:
							glUniform3f(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0], data[1], data[2]); 

							delete data;

							return true; 

							break; 

						case 4:
							glUniform4f(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0], data[1], data[2], data[3]); 

							delete data;

							return true; 

							break;

						default:
							std::cout << "\n Invalid uniform type size.";

							delete data;

							return false; 
					}

					break;

				case GL_INT:
					switch (size)
					{
						case 1:
							glUniform1i(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0]); 

							delete data;

							return true;
							
							break; 


						case 2:
							glUniform2i(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0], data[1]); 

							delete data;

							return true;
							
							break; 

						case 3:
							glUniform3i(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0], data[1], data[2]); 

							delete data;

							return true;
							
							break; 

						case 4:
							glUniform4i(this->GetUniformLocation(uniformName, this->ShaderProgramID), data[0], data[1], data[2], data[3]); 

							delete data;

							return true;

							break;

						default:
							std::cout << "\n Invalid uniform type size.";

							return false;
					}

					break; 
			}

			return false; 
		}
		Shader()
		{	
			this->ShaderProgramStrings = {
				nullptr, 
				nullptr
			};

			this->ShaderIDs = new unsigned int[2] {
				NULL, 
				NULL
			};
		}

		Shader(String vertexShaderString, String fragmentShaderString) : ShaderProgramStrings({ new char[1000], new char[1000] })	//	
		{
			strcpy(this->ShaderProgramStrings.at(0), vertexShaderString); 
			strcpy(this->ShaderProgramStrings.at(1), fragmentShaderString); 

			this->ShaderIDs = new unsigned int[2] {
				glCreateShader(GL_VERTEX_SHADER),		 
				glCreateShader(GL_FRAGMENT_SHADER)
			};
		}	

		Shader(String vertexShaderString, String fragmentShaderString, bool compile) : ShaderProgramStrings({ new char[1000], new char[1000] })	//	
		{
			strcpy(this->ShaderProgramStrings.at(0), vertexShaderString); 
			strcpy(this->ShaderProgramStrings.at(1), fragmentShaderString); 

			this->ShaderIDs = new unsigned int[2] {
				glCreateShader(GL_VERTEX_SHADER), 
				glCreateShader(GL_FRAGMENT_SHADER)
			};

			if (compile)
			{
				this->Compile();
				this->Link(); 
			}
		}
		
		Shader(std::vector<String> shaderStrings) : ShaderProgramStrings(shaderStrings)
		{
		}

		Shader(std::vector<String> shaderStrings, bool compile) : ShaderProgramStrings(shaderStrings) 
		{
			if (compile)
			{
				this->Compile();
				this->Link();
			}
		}

		~Shader()
		{
			this->DeleteShaders();
		}
	}; 
}	

