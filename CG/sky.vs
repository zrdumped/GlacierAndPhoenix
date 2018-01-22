#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;


uniform mat4 view;
uniform mat4 projection;
uniform mat4 sky;

void main()
{
	FragPos = vec3(sky * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(sky))) * aNormal;
	//Normal = aNormal;
	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	gl_Position = projection * view * sky * vec4(aPos, 1.0);
}