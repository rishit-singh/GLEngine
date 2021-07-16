#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

GLEngine::TexturePropertyObject GLEngine::DefaultTextureProperties = GLEngine::TexturePropertyObject(); 	

GLEngine::Texture::Texture(char* filePath) : FilePath(filePath), Properties(DefaultTextureProperties)
{
	stbi_set_flip_vertically_on_load(1); 

	this->TextureBuffer = stbi_load(this->FilePath, &this->Properties.Width, &this->Properties.Height, &this->Properties.BPP, 4); 	

    // std::wcout << "TextureBuffer:", this->TextureBuffer;

	glGenTextures(1, &this->ID); //	Generates a texutre buffer
	glBindTexture(GL_TEXTURE_2D, this->	ID); 	//	Binds the texture.

	this->SetTextureParameters();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->Properties.Width, this->Properties.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->TextureBuffer); 
	glGenerateMipmap(GL_TEXTURE_2D);

	if (this->TextureBuffer)
		stbi_image_free(this->TextureBuffer);

	this->Unbind(); 
}

void GLEngine::Texture::SetTextureParameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void GLEngine::Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);	//	Texture slot 0
	glBindTexture(GL_TEXTURE_2D, this->ID); 
}

void GLEngine::Texture::Bind(unsigned int slots)
{
	glActiveTexture(GL_TEXTURE0 + slots);
	glBindTexture(GL_TEXTURE_2D, this->ID); 
}

void GLEngine::Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool GLEngine::Texture::SendToShader(GLEngine::Shader* shader)
{
	//	Sends the current bound texture slot as uTextureSlot to the provided shader
	//	ToDo:	Pass texture slots
	return shader->SetUniformValue<int>("uTextureSlot", GL_INT, new int[1] { (int)this->ID }, 1);
}
