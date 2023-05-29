#version 460 core

out vec4 FragColor;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D containerTexture;
uniform sampler2D avatarTexture;

uniform float mixvalue;

void main()
{
	FragColor = mix(texture(containerTexture, texCoord), texture(avatarTexture, texCoord), mixvalue);
}