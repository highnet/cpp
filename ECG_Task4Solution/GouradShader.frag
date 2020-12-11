//fragment shader
#version 430
out vec4 FragColor;

in vec3 LightingColor; 

uniform vec3 materialColor;

void main()
{
   FragColor = vec4(LightingColor * materialColor, 1.0);
}