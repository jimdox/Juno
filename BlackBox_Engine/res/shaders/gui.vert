#version 400 core
// for 2D gui components
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 texColor;
layout(location = 2) in vec2 texCoord;

out vec3 passColor;
out vec2 passTexCoord;

uniform mat4 transformMat;

void main()
{
        gl_Position = transformMat * vec4(vertexPosition, 1.0);
		passColor = texColor;
        passTexCoord = texCoord;

}






