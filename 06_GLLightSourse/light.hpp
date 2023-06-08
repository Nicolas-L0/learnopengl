#ifndef LIGHT_H
#define LIGHT_H

#include "shader.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

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

	PointLight(vec3 pos, vec3 col, float a, float d, float s, float c, float l, float q);
	void setPointLight(Shader shader, int i);
};

class SpotLight :public Light {
public:
	vec3 position;
	vec3 direction;

	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	SpotLight(vec3 pos, vec3 dir, vec3 col, float cut, float ocut, float a, float d, float s, float c, float l, float q);
	void setSpotLight(Shader shader);
	void updateLight(vec3 pos, vec3 dir);
};
#endif