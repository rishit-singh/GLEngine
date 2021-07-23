#ifndef TEXTURE_H

#include <GL/glew.h>
#include "globjects.h"
#include "globals.h"
#include "shader.h"
#include "window.h"


namespace GLEngine
{
	enum TextureType
	{
		Texture2D,
		Texture3D
	}; 

	struct TexturePropertyObject	//	Stores scale and other visual properties of a texture
	{
		int Height, Width, BPP; 

		bool Verify(); // Checks if the current instance of the object is valid

		TexturePropertyObject() : Height(0), Width(0), BPP(0)
		{
		}

		TexturePropertyObject(int height, int width) : Height(height), Width(width), BPP(0)
		{
		}

		TexturePropertyObject(int height, int width, int bpp) : Height(height), Width(width), BPP(bpp)
		{
		}
	}; 

	extern TexturePropertyObject DefaultTextureProperties;	//	Default texture properties for textures with unspecified texture properties

	class Texture   //   Stores the texture information 
	{
	private:
		String FilePath;	//	Path to the texture file
		
		void SetTextureParameters(); 	//	Sets up the required texture paramneters.
		
		// Todo: implement an overload SetTextureParameter(TextureType)

	public:		
		unsigned int ID; //	Texture buffer ID
		unsigned char* TextureBuffer;	//	Stores the image data.
		
		unsigned int Slot; 

		TexturePropertyObject Properties; //	Visual properties of the texture

		bool IsValid();	//	Checks if the current texture instance is valid.

		void Bind();	//	Binds the texture to slot 0. 
		void Bind(unsigned int);	//	Binds the texture to the specified texture slots 
		void Unbind();	//	Unbinds the texture
		bool SendToShader(Shader* shader);	//	Sends the required texture data from current instance for raterization to the specified texture	

		Texture() : ID(-1), TextureBuffer(nullptr), Properties(DefaultTextureProperties), FilePath(nullptr)
		{
		}
	
		Texture(char* filePath);	//	To load a texture from an image file 

		~Texture()
		{
			glDeleteTextures(1, &this->ID); 

			// delete &this->Properties;
		}
	};
}

#endif
