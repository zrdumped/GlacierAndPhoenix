#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


struct PointLight {
    vec3 position;
	vec3 lightColor;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define fireNum 5
// texture samplers
uniform sampler2D texture3;
uniform PointLight lights[fireNum];
uniform PointLight wings[fireNum];

uniform vec3 lightColor;
uniform vec3 lightDirection;
uniform vec3 lightPos;
uniform vec3 viewPos; 
//uniform vec3 objectColor;

uniform float constant;
uniform float linear;
uniform float quadratic;

uniform float FogMaxDist;
uniform float FogMinDist;
uniform vec4 FogColor;

uniform bool isDay;

vec3 CalSun( vec3 vieDir, vec3 norm);
vec3 CalFire(PointLight lightw, vec3 vieDir, vec3 norm);

void main()
{
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 norm = normalize(Normal);
	vec3 tmp = CalSun(viewDir, norm);

	for(int i = 0; i < fireNum; i++)
        tmp += CalFire(lights[i], viewDir, norm);    
	for(int i = 0; i < fireNum; i++)
		tmp += CalFire(wings[i], viewDir, norm);    

	//fire
	vec4 result;
	if(isDay)
		result = vec4(tmp, 1.0f) *  texture(texture3, TexCoord) * 0.8 ;//2;//0.8;
	else
		result = vec4(tmp, 1.0f) *  texture(texture3, TexCoord) ;
	//vec4 result = vec4((ambient + diffuse), 1.0f) *  texture(texture3, TexCoord);

	float dist = length(FragPos - viewPos);
	float fogFactor = (FogMaxDist - dist )/ (FogMaxDist - FogMinDist);
	fogFactor = clamp(fogFactor, 0.0, 1.0);

	FragColor =  mix(FogColor, result, fogFactor);

    //FragColor = result;
}

vec3 CalFire(PointLight light, vec3 viewDir, vec3 norm){
    vec3 lightDir = normalize(light.position - FragPos);
    // diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(texture3, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture3, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(texture3, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.lightColor;
}

vec3 CalSun(vec3 viewDir, vec3 norm){
	float ambientStrength = 0.1f;
	//else
	//	ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    
	vec3 lightDir;
	if(isDay){
		lightDir = normalize(-lightDirection);  
		}
	else{
		lightDir = normalize(-lightPos - FragPos);

		}
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// specular
    float specularStrength = 0.1f;
    vec3 reflectDir = reflect(-lightDir, norm);  

	vec3 specular;
	if(!isDay){
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    specular = specularStrength * spec * lightColor;  

	
	// attenuation
    float distance    = length(-lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse   *= attenuation;
    specular *= attenuation;   
	}else{

	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 1.0f);
	specular = vec3(0.3) * spec;

	}

	return (ambient + diffuse + specular);
}