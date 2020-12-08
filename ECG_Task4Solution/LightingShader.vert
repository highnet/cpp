//vertex shader
#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 Normal;

void main()
{
    gl_Position = proj* view * model * vec4(position, 1.0);
    Normal = normal;
} 