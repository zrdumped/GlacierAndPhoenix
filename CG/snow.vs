#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 aInstanceMatrix;
layout (location = 7) in vec4 color;

//out vec2 TexCoords;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec4 particlecolor;
//out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
//uniform mat4 model;
uniform float scale;



void main()
{
    FragPos = vec3(aInstanceMatrix*vec4((aPos.xyz * scale),1.0));
    Normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    gl_Position = projection * view * aInstanceMatrix*vec4((aPos.xyz * scale) ,1.0);
    particlecolor = color;
}

