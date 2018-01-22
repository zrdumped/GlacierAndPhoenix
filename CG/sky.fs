#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform float constant;
uniform float linear;
uniform float quadratic;

// texture samplers
//uniform sampler2D texture3;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos; 
uniform vec3 objectColor;
uniform float specularStrength;
uniform vec3 offset_RGB;
uniform float radius;

uniform float FogMaxDist;
uniform float FogMinDist;
uniform vec4 FogColor;

void main()
{
	// ambient
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    //vec3 specular = specularStrength * spec * lightColor;  

	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), radius);
	vec3 specular = vec3(0.3) * spec;

	// attenuation
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
	//vec3 color = vec3(objectColor.x + attenuation * 1.0f, objectColor.y + attenuation * 1.0f, objectColor.z - attenuation * 10.0f);

	vec3  result = (ambient + diffuse + specular) * objectColor * offset_RGB ;//* 5.0f;
	//result.x *= 5.0f;result.y *= 3.0f;
	//result.z *= 30;
	//vec4 result = vec4((ambient + diffuse), 1.0f) *  texture(texture3, TexCoord);
    //FragColor = vec4(result, 1.0f);

	float dist = length(FragPos - viewPos);
	float fogFactor = (FogMaxDist - dist )/ (FogMaxDist - FogMinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	FragColor =  mix(FogColor, vec4(result, 1.0f), fogFactor);
}
