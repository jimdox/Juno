#version 400 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D testTexture;

void main()
{
    FragColor = texture(testTexture, TexCoord);
}