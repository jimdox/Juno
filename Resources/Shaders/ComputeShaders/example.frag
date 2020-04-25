#version 430 core 

in float pass_scale;
in float pass_speed; 
in vec3 pass_position;

out vec4 outColor;

void main()
{
	vec3 pos = normalize(pass_position);
	float r = max(pos.x, 0.3);
	float g = max(pos.y, 0.3);
	float b = max(pos.z, 0.3);

	outColor = vec4(r, g, b, 1.0);
}