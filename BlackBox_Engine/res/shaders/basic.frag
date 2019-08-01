#version 400 core
in vec3 passColor;
in vec2 passTexCoord;

out vec4 FragColor; 

uniform sampler2D diffuse;
uniform sampler2D normal;
	
void main()
{
        FragColor = mix(texture(normal, passTexCoord), texture(normal, passTexCoord), 0.9);




}
