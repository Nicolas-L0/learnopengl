#include "light.hpp"


void Light::updateLight() {
	ambient = a * color;
	diffuse = d * color;
	specular = s * color;
}

DirLight::DirLight(vec3 dir, vec3 col, float a, float d, float s) {
	color = col;
	this->a = a;
	this->d = d;
	this->s = s;

	direction = normalize(dir);

	updateLight();
}

PointLight::PointLight(vec3 pos, vec3 col, float a, float d, float s, float c, float l, float q) {
	color = col;
	this->a = a;
	this->d = d;
	this->s = s;

	constant = c;
	linear = l;
	quadratic = q;

	position = pos;

	updateLight();
}

SpotLight::SpotLight(vec3 pos, vec3 dir, vec3 col, float cut, float ocut, float a, float d, float s, float c, float l, float q) {
	color = col;
	this->a = a;
	this->d = d;
	this->s = s;

	constant = c;
	linear = l;
	quadratic = q;

	position = pos;
	direction = normalize(dir);

	cutOff = cut;
	outerCutOff = ocut;

	updateLight(position, direction);
}

void SpotLight::updateLight(vec3 pos, vec3 dir) {
	ambient = a * color;
	diffuse = d * color;
	specular = s * color;

	position = pos;
	direction = dir;
}