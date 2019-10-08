#version 400 core
in vec3 passColor;
in vec2 passTexCoord;

out vec4 outColor; 

uniform sampler2D diffuse;
uniform sampler2D normal;
	
void main()
{
        outColor = mix(texture(normal, passTexCoord), texture(normal, passTexCoord), 0.9);



}
