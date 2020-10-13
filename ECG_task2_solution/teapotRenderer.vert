//vertex shader
#version 430 

layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

out vec4 vertexColor; // specify a color output to the fragment shader

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0); // position
    vertexColor = vec4(1.0, 1.0, 1.0, 1.0); // default color white
}
