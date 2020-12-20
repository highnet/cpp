//fragment shader
#version 430

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 Uv;

uniform vec3 materialColor;
uniform vec3 viewPos;

uniform vec3 pLightColor;
uniform vec3 pLightPosition;

uniform vec3 dLightColor;
uniform vec3 dLightDirection;

uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

uniform float k_constant;
uniform float k_linear;
uniform float k_quadratic;

uniform sampler2D colorTexture;


void main()
{
    vec3 result;

    // ambient
    float ambientStrength = 1.0f;
    vec3 ambient = k_ambient * ambientStrength * pLightColor;    
    
     // diffuse 
    float diffuseStrength = 1.0f;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pLightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = k_diffuse * diffuseStrength * diff * pLightColor;
    
    // specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);
    vec3 specular = k_specular * specularStrength * spec * pLightColor; 
    
    float distance    = length(pLightPosition - FragPos);
    float attenuation = 1.0 / (k_constant + k_linear * distance + k_quadratic * (distance * distance)); 

    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

    result = (ambient + diffuse + specular) * materialColor;


    //DIRECTIONAL LIGHT
    // ambient
    ambient = k_ambient * dLightColor;    
    
    // diffuse 
     norm = normalize(Normal);
     lightDir = normalize(-dLightDirection);
     diff = max(dot(norm, lightDir), 0.0);
     diffuse = k_diffuse  * diff * dLightColor;
    
    // specular
    viewDir = normalize(viewPos-FragPos);
    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 10);
    specular = k_specular * spec * dLightColor;

    result += (ambient + diffuse + specular) * materialColor;

    vec3 color = result * texture(colorTexture, Uv).rgb;
    FragColor = vec4(color, 1.0);
}