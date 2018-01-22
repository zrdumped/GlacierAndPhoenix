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
uniform float colorPort;
uniform vec4 startColor;
uniform vec4 endColor;


void main()
{/*
    //ambient
    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f) ;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = diff * vec3(1.0f,1.0f,1.0f) ;
    
    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = vec3(1.0f,1.0f,1.0f) * spec;
    
	//vec4 finalColor;
	float dist = distance(FragPos.xyz,vec3(0.0,0.0,0.0));
	if(dist > textureRadius) discard;
	//else finalColor = particlecolor;
    vec4 result = vec4((ambient+ specular + diffuse),1.0f)*vec4(1.0f, 1.0f, 1.0f, 1.0f) * particlecolor;*/

	
	/*vec4 colorTL = texture(texture0, TexCoord);//进行纹理采样
	if(colorTL.x < 0.1) colorTL.a = 0;
    vec4 colorT = particlecolor;//颜色变量
    float disT=distance(FragPos.xyz,vec3(0.0,0.0,0.0));//计算当前片元与中心点的距离
    float tampFactor=(1.0-disT/textureRadius)*colorPort;//计算片元颜色插值因子
    vec4 factor4=vec4(tampFactor,tampFactor,tampFactor,tampFactor);
    //colorT=clamp(factor4,endColor,startColor);//进行颜色插值
    colorT=colorT*colorTL.a;  //结合采样出的透明度计算最终颜色
    FragColor=colorT; //将计算出来的片元颜色传给渲染管线*/

	vec4 colorTL = texture(texture0, TexCoord);//进行纹理采样
	if(colorTL.x < 0.1) colorTL.a = 0;
	vec4 finalColor = particlecolor * colorTL;
	FragColor= finalColor;

	//FragColor =  result;
	//vec4 final = vec4(colorFog, 1.0f) * particlecolor;

}


