#version 400 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 texColor;
layout(location = 2) in vec2 texCoord;

out vec3 passColor;
out vec2 passTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
        gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
		passColor = texColor;
        passTexCoord = texCoord;

}






