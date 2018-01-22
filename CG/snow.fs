#version 330 core
in vec2 TexCoord;
//in vec4 ParticleColor;
in vec3 FragPos;
in vec3 Normal;
in vec4 particlecolor;
out vec4 FragColor;

//uniform sampler2D sprite;
//uniform vec3 lightColor;
//uniform sampler2D texturep;
uniform sampler2D texture0;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float shininess;
uniform bool isDay;

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
uniform PointLight lights[fireNum];
uniform PointLight wings[fireNum];



vec3 CalFire(PointLight lightw, vec3 vieDir, vec3 norm);

void main()
{
	vec4 result;
	if(isDay){
    //ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f);
    result = vec4((ambient),1.0f)*vec4(1.0f, 1.0f, 1.0f, 1.0f)* particlecolor * texture(texture0, TexCoord);

	}
	else{

	/*
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f);

	    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * vec3(1.0f,1.0f,1.0f) ;
    
    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = vec3(1.0f,1.0f,1.0f) * spec ;
	*/
	vec3 tmp = vec3(0);
	 vec3 norm = normalize(Normal);
	 vec3 viewDir = normalize(viewPos - FragPos);
	for(int i = 0; i < fireNum; i++)
        tmp += CalFire(lights[i], viewDir, norm);    
	for(int i = 0; i < fireNum; i++)
		tmp += CalFire(wings[i], viewDir, norm);    

    result = vec4(tmp,1.0f)*vec4(1.0f, 1.0f, 1.0f, 1.0f)* particlecolor * texture(texture0, TexCoord);

	}
	//vec4 final = vec4(colorFog, 1.0f) * particlecolor;
	FragColor =  result;
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
    vec3 ambient = light.ambient * particlecolor.xyz* vec3(texture(texture0, TexCoord));
    vec3 diffuse = light.diffuse * diff * particlecolor.xyz* vec3(texture(texture0, TexCoord));
    vec3 specular = light.specular * spec * particlecolor.xyz* vec3(texture(texture0, TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) * light.lightColor;
}
