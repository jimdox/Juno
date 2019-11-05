#version 450 core
in vec3 position;

out vec3 pass_texCoordinates;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    pass_texCoordinates = position;
    gl_Position = projection * view * vec4(position, 1.0);
}