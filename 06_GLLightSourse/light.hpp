#ifndef LIGHT_H
#define LIGHT_H

#include "shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static vec3 lightAttenuation[] = {
	vec3(1.0f, 0.7f, 1.8f),
	vec3(1.0f, 0.35f, 0.44f),
	vec3(1.0f, 0.22f, 0.20f),
	vec3(1.0f, 0.14f, 0.07f),
	vec3(1.0f, 0.09f, 0.032f),
	vec3(1.0f, 0.07f, 0.017f),
	vec3(1.0f, 0.045f, 0.0075f),
	vec3(1.0f, 0.027f, 0.0028f),
	vec3(1.0f, 0.022f, 0.0019f),
	vec3(1.0f, 0.014f, 0.0007f),
	vec3(1.0f, 0.007f, 0.0002f),
	vec3(1.0f, 0.0014f, 0.000007f)
};
static const char* lightAttenuation_distance_name[] = {
	"7", "13", "20", "32", "50", "65", "100", "160", "200", "325", "600", "3250"
};
enum Distance_ENUM {
	D7, D13, D20, D32, D50, D65, D100, D160, D200, D325, D600, D3250, Distance_ENUM_LAST
};

class Light {
public:
	vec3 color;

	float a, d, s;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	void updateLight();
};

class DirLight :public Light {
public:
	vec3 direction;

	DirLight(vec3 dir, vec3 col, float a, float d, float s);
	void setDirLight(Shader shader);
};

class PointLight :public Light {
public:
	vec3 position;
	float constant;
	float linear;
	float quadratic;

	Distance_ENUM DISTANCE;

	PointLight(vec3 pos, vec3 col, float a, float d, float s, Distance_ENUM dis);
	void setPointLight(Shader shader, int i);
	void updateLight();
};

class SpotLight :public Light {
public:
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;
	float AcutOff;
	float AouterCutOff;

	float constant;
	float linear;
	float quadratic;

	Distance_ENUM DISTANCE;

	SpotLight(vec3 pos, vec3 dir, vec3 col, float cut, float ocut, float a, float d, float s, Distance_ENUM dis);
	void setSpotLight(Shader shader);
	void updateLight(vec3 pos, vec3 dir);
};
#endif