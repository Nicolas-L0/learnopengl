#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float time;
uniform mat4 trans;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outColor;
out vec2 texCoord;

float pi = 3.14;

void main()
{
	float xoffset = cos(2 * pi * time / 2.0) / 2.0;
	float yoffset = sin(2 * pi * time / 2.0) / 2.0;
	//gl_Position = trans * vec4(aPos.x + xoffset, aPos.y + yoffset, aPos.z, 1.0);
	//gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = projection * view * model * vec4(aPos.x + xoffset, aPos.y + yoffset, aPos.z, 1.0);

	//outColor = aColor;
	outColor = vec3(aColor.r * (xoffset + 0.5), aColor.g * (yoffset + 0.5), aColor.b / 2.0 + 0.25);
	texCoord = aTexCoord;
}