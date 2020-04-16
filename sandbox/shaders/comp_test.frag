#version 430 core 

in float pass_scale;
in float pass_speed; 
in vec3 pass_position;
out vec4 outColor;

void main()
{
	float minBrightness = 0.3f;
	float c = max((pass_speed), minBrightness);	
	float g = pass_speed;
	float b = pass_speed;
	vec3 colors = normalize(pass_position);
	outColor = vec4(colors, 1.0);
}