#version 460 core

out vec4 FragColor;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	FragColor = mix(texture(ourTexture, texCoord), vec4(1.0f, 0.5f, 0.2f, 1.0f), 0.2);
}