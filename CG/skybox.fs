#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform vec3 lightColor;

void main()
{
	// ambient
    float ambientStrength = 1.0f;
    vec3 ambient = ambientStrength * lightColor;    

	vec4 result = vec4(ambient , 1.0f) *  texture(skybox, TexCoords);
	//vec4 result = vec4((ambient + diffuse), 1.0f) *  texture(texture3, TexCoord);
    FragColor = result;
}