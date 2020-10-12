//vertex shader
#version 430 
layout (location = 0) in vec3 aPos; // the position variable has attribute position 0
  
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0); // position
    vertexColor = vec4(1.0, 1.0, 1.0, 1.0); // default color white
}
