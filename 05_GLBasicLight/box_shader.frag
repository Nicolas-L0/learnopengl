#version 460 core

in vec3 outColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;
uniform float mixvalue;
uniform vec3 viewPos;
uniform float time;
uniform sampler2D avatarTexture;
uniform sampler2D matrixTexture;

out vec4 FragColor;

void main()
{
	// Ambient lighting
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;

	// Diffuse lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normalize(Normal), lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * mix(texture(material.diffuse, texCoord).rgb, texture(matrixTexture, vec2(texCoord.x, texCoord.y + time * 0.5)).rgb, 0.5);

	// Specular Lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalize(Normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	/* Frag Color*/
	FragColor = mix(vec4(ambient + diffuse + specular, 1.0f), texture(avatarTexture, texCoord), mixvalue);
}