//fragment shader
#version 430

out vec4 FragColor;

uniform vec4 outColor;

void main()
{
    FragColor = outColor;
}