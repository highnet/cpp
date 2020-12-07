//fragment shader
#version 430

out vec4 FragColor;

uniform vec4 objectColor;
uniform vec4 lightColor;

void main()
{
    FragColor = objectColor * lightColor;
}
