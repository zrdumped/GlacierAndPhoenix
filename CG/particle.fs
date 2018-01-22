#version 330 core
//in vec2 TexCoords;
//in vec4 ParticleColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec4 particlecolor;
out vec4 FragColor;

//uniform sampler2D sprite;
//uniform vec3 lightColor;
uniform sampler2D texturep;


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float shininess;
void main()
{
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength*vec3(1.0f,1.0f,1.0f) * particlecolor.xyz;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * vec3(1.0f,1.0f,1.0f) * particlecolor.xyz;
    
    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = vec3(1.0f,1.0f,1.0f) * (spec * particlecolor.xyz);
    
    vec4 result = vec4((ambient+diffuse+specular), 1.0f)*texture(texturep,TexCoord)*particlecolor;
    FragColor = result;
}


