//fragment shader
#version 430
out vec4 FragColor;
  
uniform vec4 outColor; // we set this variable in the OpenGL code.

void main()
{
    FragColor = outColor;
}   
