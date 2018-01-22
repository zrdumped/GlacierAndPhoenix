#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;


uniform mat4 view;
uniform mat4 projection;
uniform mat4 floor;

void main()
{
	FragPos = vec3(floor * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(floor))) * aNormal;
	//Normal = aNormal;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
	gl_Position = projection * view * floor * vec4(aPos, 1.0);
}