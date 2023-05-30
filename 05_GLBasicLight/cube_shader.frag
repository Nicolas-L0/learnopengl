#version 460 core

in vec3 outColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform int shininess;

void main()
{
	// TexColor
	vec4 texColor = vec4(outColor, 1.0);

	// Ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse lighting
	vec3 lightDir = normalize(lightPos - FragPos);
	float costheta = dot(normalize(Normal), lightDir);
	float diff = max(costheta, 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular Lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalize(Normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), pow(2,shininess));
	vec3 specular = specularStrength * spec * lightColor;

	/* Frag Color*/
	FragColor = vec4(ambient + diffuse + specular, 1.0f) * texColor;
}