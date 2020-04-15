#version 430 core 

in float pass_scale;
in float pass_speed; 

out vec4 outColor;

void main()
{
	float minBrightness = 0.6f;
	float r = max((pass_speed), minBrightness);	
	float g = max((pass_speed), minBrightness);
	float b = max((pass_speed), minBrightness);

	outColor = vec4(r,g,b, 1.0);
}