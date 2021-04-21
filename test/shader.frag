#version 460 core

out vec4 FragColor;

uniform vec4 Color; 

void main()
{
	FragColor = Color; //vec4(1.0f, 0.5f,  0.2f, 1.0f);
}
