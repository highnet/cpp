//fragment shader
#version 430

out vec4 FragColor;

uniform vec4 objectColor;
uniform vec4 lightColor;
uniform float ambientStrength;


void main()
{
    vec4 ambient = ambientStrength * lightColor;
    vec4 result = ambient * objectColor;
    FragColor = result;
}