//fragment shader
#version 430

in vec3 Normal;

out vec4 FragColor;

uniform vec4 objectColor;
uniform vec4 lightColor;
uniform float ambientStrength;
uniform vec3 lightPos;


void main()
{
    vec4 ambient = ambientStrength * lightColor;
    vec4 result = ambient * objectColor;
    FragColor = result;
}