#version 450 core
in vec3 pass_texCoordinates;

out vec3 output_Color;

uniform samplerCube s_cube;

void main()
{
    output_Color = texture(s_cube, pass_texCoordinates);
}