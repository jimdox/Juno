#version 450 core
in vec3 pass_texCoordinates;

out vec3 output_Color;

uniform samplerCube skybox;

void main()
{
    output_Color = texture(skybox, pass_texCoordinates);
}