//fragment shader
#version 430

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 materialColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

void main()
{
    // ambient
    float ambientStrength = 1.0f;
    vec3 ambient = k_ambient * ambientStrength * lightColor;    
    
     // diffuse 
    float diffuseStrength = 1.0f;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = k_diffuse * diffuseStrength * diff * lightColor;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256); // last value is the material shinyness
    vec3 specular = specularStrength * spec * lightColor; 
    
    vec3 result = k_specular * (ambient + diffuse + specular) * materialColor;
    FragColor = vec4(result, 1.0);
}