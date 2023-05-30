#version 460 core

out vec4 FragColor;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D containerTexture;
uniform sampler2D avatarTexture;
uniform vec3 lightColor;

uniform float mixvalue;

void main()
{
	FragColor = vec4(lightColor, 1.0f) * mix(texture(containerTexture, texCoord), texture(avatarTexture, texCoord), mixvalue);
}