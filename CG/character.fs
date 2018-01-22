#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


// texture samplers
uniform sampler2D texture0;
//uniform sampler2D texture2;

uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);

	// ambient
   /* float ambientStrength = 1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec4 result = vec4((ambient + diffuse), 1.0f)*  texture(texture0, TexCoord);*/
    FragColor = texture(texture0, TexCoord);
}
