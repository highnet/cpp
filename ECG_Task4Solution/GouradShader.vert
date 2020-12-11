//vertex shader
#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 LightingColor; // resulting color from lighting calculations

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

void main()
{
    gl_Position = proj * view * model * vec4(position, 1.0);
    
    vec3 Position = vec3(model * vec4(position, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * normal;
    
    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = k_ambient * ambientStrength * lightColor;
  	
    // diffuse
    float diffuseStrength = 1.0;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = k_diffuse * diffuseStrength * diff * lightColor;
    
    // specular
    float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;      

    LightingColor = k_specular + ambient + diffuse + specular;
}