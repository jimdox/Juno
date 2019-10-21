#version 450 core
in vec3 position;
in vec3 normal;
in vec2 texCoordinates;

out vec2 pass_texCoordinates;
out vec3 surfaceNormal;
out vec3 lightDir;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;

void main()
{
        vec4 globalPosition = transformationMatrix * vec4(position, 1.0);
        gl_Position = projectionMatrix * viewMatrix * globalPosition;
        pass_texCoordinates = texCoordinates;

        surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz;
        lightDir = lightPosition - (globalPosition).xyz;
}






