  //vertex shader
#version 430 

in vec3 position;

out vec4 vertexColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{

    gl_Position = proj * view * model * vec4(position, 1.0); // position
    vertexColor = vec4(0.0, 1.0, 1.0, 1.0); // default color 
}