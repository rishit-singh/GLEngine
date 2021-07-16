#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uTextureCoordinates; 
// layout (location = 2) in vec3 Color;

out vec2 TextureCoordinates;  
// out vec3 oColor;

void main()
{
	gl_Position = vec4(aPos, 1.0);

	TextureCoordinates = uTextureCoordinates;

	// oColor = Color;
}
