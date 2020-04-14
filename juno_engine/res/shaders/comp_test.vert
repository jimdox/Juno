#version 430 core

layout(location = 0) in vec3 particlePos;
layout(location = 1) in float life;
uniform mat4 projection;
uniform mat4 view;

out float pass_color;

void main()
{
    pass_color = life;
    gl_Position = projection * view * vec4(particlePos,1.0);
}
