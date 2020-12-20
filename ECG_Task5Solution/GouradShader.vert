//vertex shader
#version 430
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 LightingColor; // resulting color from lighting calculations
uniform vec3 viewPos;

uniform vec3 pLightPosition;
uniform vec3 pLightColor;

uniform vec3 dLightColor;
uniform vec3 dLightDirection;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

uniform float k_constant;
uniform float k_linear;
uniform float k_quadratic;

void main()
{

    vec3 result;

    gl_Position = proj * view * model * vec4(position, 1.0);
    
    vec3 Position = vec3(model * vec4(position, 1.0));
    vec3 Normal = mat3(transpose(inverse(model))) * normal;
    
    //POINT LIGHT
    // ambient
    float ambientStrength = 1.0;
    vec3 ambient = k_ambient * ambientStrength * pLightColor;
  	
    // diffuse
    float diffuseStrength = 1.0;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pLightPosition - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = k_diffuse * diffuseStrength * diff * pLightColor;
    
    // specular
    float specularStrength = 1.0; 
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);
    vec3 specular = k_specular * specularStrength * spec * pLightColor;      

    float distance    = length(pLightPosition - position);
    float attenuation = 1.0 / (k_constant + k_linear * distance + k_quadratic * (distance * distance)); 

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    result = (ambient + diffuse + specular);

    //DIRECTIONAL LIGHT
    // ambient
     ambientStrength = 1.0;
     ambient = k_ambient * ambientStrength * dLightColor;
  	
    // diffuse
     diffuseStrength = 1.0;
     norm = normalize(Normal);
     lightDir = normalize(-dLightDirection);
     diff = max(dot(norm, lightDir), 0.0);
     diffuse = k_diffuse * diffuseStrength * diff * dLightColor;
    
    // specular
     specularStrength = 1.0;
     viewDir = normalize(viewPos - Position);
     reflectDir = reflect(-lightDir, norm);  
     spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);
     specular = k_specular * specularStrength * spec * dLightColor;      

    result += ambient + diffuse + specular;

    LightingColor = result;

}