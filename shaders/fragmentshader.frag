#version 400

out vec4 FragmentColor; 

in vec2 TextureCoordinates;

// uniform vec4 uFragmentColor;
uniform sampler2D uTextureSlot; 

void main()
{
	vec4 TextureColor = texture(uTextureSlot, TextureCoordinates);

	FragmentColor =	TextureColor;
}	
