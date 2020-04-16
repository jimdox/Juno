#version 430 core 

in float pass_scale;
in float pass_speed; 

out vec4 outColor;

void main()
{
	float minBrightness = 0.3f;
	float c = max((pass_speed), minBrightness);	
	float g = pass_speed;
	float b = pass_speed;

	outColor = vec4(c, c, 0.9, 1.0);
}