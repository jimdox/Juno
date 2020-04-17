#version 430 core 

in float pass_speed; 
in vec3 pass_position;

out vec4 outColor;

void main()
{
	vec3 pos = normalize(pass_position);
	float r = min(pos.x, 0.5);
	float g = min(pos.y, 0.5);
	float b = min(pos.z, 0.5);

	outColor = vec4(r, g, b, 1.0);
}