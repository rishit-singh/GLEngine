#version 400

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 uTextureCoordinates; 
// layout (location = 2) in vec3 Color;

out vec2 TextureCoordinates;  

// uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// out vec3 oColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	TextureCoordinates = uTextureCoordinates;

	// oColor = Color;
}