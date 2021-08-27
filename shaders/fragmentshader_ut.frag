#version 400

out vec4 FragmentColor; 

// in vec2 TextureCoordinates;

// in vec3 oColor;

// uniform vec4 uFragmentColor;

uniform sampler2D uTextureSlot;
 
void main()
{
	// vec4 TextureColor = texture(uTextureSlot, TextureCoordinates);

	FragmentColor = vec4(0, 0.68, 1, 1);
	// FragmentColor = vec4(1.0, 1.0, 1.0, 1.0);
}	
