#version 460 core

out vec4 FragColor;

in vec3 outColor;
in vec2 texCoord;

uniform sampler2D ourTexture;

void main()
{
	//FragColor = vec4(outColor, 1.0);
	FragColor = mix(texture(ourTexture, texCoord) , vec4(outColor, 1.0), 0.5);
}