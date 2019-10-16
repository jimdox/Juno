#version 450 core
in vec3 pass_color;
in vec2 pass_texCoordinates;

out vec4 output_color; 

uniform sampler2D diffuse;
	
void main()
{
        output_color = texture(diffuse, pass_texCoordinates);


}
