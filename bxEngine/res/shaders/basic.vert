#version 450 core
in vec3 position;
in vec2 texCoordinates;

out vec3 pass_color;
out vec2 pass_texCoordinates;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
        gl_Position =   projectionMatrix * viewMatrix * transformationMatrix * vec4(position, 1.0);
        pass_texCoordinates = texCoordinates;
        pass_color = vec3(position.x+0.5, 0.0, position.y+0.5);

}






