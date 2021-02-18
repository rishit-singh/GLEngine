#version 400

out vec4  FragmentColor; 
uniform vec4 uFragmentColor;

void main()
{
	FragmentColor = uFragmentColor;
}
	