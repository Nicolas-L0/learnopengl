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

struct DirLight{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointlight;
uniform SpotLight spotlight;
uniform float mixvalue;
uniform vec3 viewPos;
uniform float time;
uniform sampler2D avatarTexture;
uniform sampler2D matrixTexture;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewdir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewdir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewdir);

void main()
{

	vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

	/* Lighting */
	vec3 result = vec3(0.0f);
	result += CalcDirLight(dirlight, norm, viewDir);
	result += CalcPointLight(pointlight, norm, viewDir);
	result += CalcSpotLight(spotlight, norm, viewDir);

	/* Frag Color*/
	FragColor = mix(vec4(result, 1.0f), texture(avatarTexture, texCoord), mixvalue);

}
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewdir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewdir, reflectDir), 0.0), material.shininess);

    // combine
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * mix(texture(material.diffuse, texCoord).rgb, texture(matrixTexture, vec2(texCoord.x, texCoord.y + time * 0.5)).rgb, 0.5);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewdir){

	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewdir, reflectDir), 0.0), material.shininess);

	// combine
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * mix(texture(material.diffuse, texCoord).rgb, texture(matrixTexture, vec2(texCoord.x, texCoord.y + time * 0.5)).rgb, 0.5);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewdir){
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewdir, reflectDir), 0.0), material.shininess);

	// combine
	vec3 ambient = light.ambient * texture(material.diffuse, texCoord).rgb;
	vec3 diffuse = light.diffuse * diff * mix(texture(material.diffuse, texCoord).rgb, texture(matrixTexture, vec2(texCoord.x, texCoord.y + time * 0.5)).rgb, 0.5);
	vec3 specular = light.specular * spec * texture(material.specular, texCoord).rgb;

	// attenuation & intensity
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	float theta = dot(lightDir, normalize(-light.direction)); 
	float intensity = clamp((theta - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}