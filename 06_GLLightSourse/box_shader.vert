#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec3 outColor;
out vec2 texCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	Normal = normalMatrix * aNormal;
	outColor = aColor;
	texCoord = aTexCoord;
	FragPos = vec3(model * vec4(aPos, 1.0));
}