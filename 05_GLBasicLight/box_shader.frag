#version 460 core

in vec3 outColor;
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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
uniform sampler2D containerTexture;
uniform sampler2D avatarTexture;

out vec4 FragColor;

void main()
{
	// TexColor
	vec4 texColor = mix(texture(containerTexture, texCoord), texture(avatarTexture, texCoord), mixvalue);

	// Ambient lighting
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse lighting
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normalize(Normal), lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular Lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalize(Normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	/* Frag Color*/
	FragColor = vec4(ambient + diffuse + specular, 1.0f) * texColor;
}