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
void DirLight::setDirLight(Shader shader) {
	shader.setVec3("dirlight.direction",	&direction);
	shader.setVec3("dirlight.ambient",		&ambient);
	shader.setVec3("dirlight.diffuse",		&diffuse);
	shader.setVec3("dirlight.specular",		&specular);
}


PointLight::PointLight(vec3 pos, vec3 col, float a, float d, float s, Distance_ENUM dis) {
	color = col;
	this->a = a;
	this->d = d;
	this->s = s;

	position = pos;
	DISTANCE = dis;

	updateLight();
}
void PointLight::setPointLight(Shader shader, int i) {
	std::string index = "[" + std::to_string(i) + "]";

	shader.setVec3 ("pointlights" + index + ".position",	&position);
	shader.setVec3 ("pointlights" + index + ".ambient",		&ambient);
	shader.setVec3 ("pointlights" + index + ".diffuse",		&diffuse);
	shader.setVec3 ("pointlights" + index + ".specular",	&specular);
	shader.setFloat("pointlights" + index + ".constant",	constant);
	shader.setFloat("pointlights" + index + ".linear",		linear);
	shader.setFloat("pointlights" + index + ".quadratic",	quadratic);
}
void PointLight::updateLight() {
	ambient = a * color;
	diffuse = d * color;
	specular = s * color;

	constant = lightAttenuation[DISTANCE].x;
	linear = lightAttenuation[DISTANCE].y;
	quadratic = lightAttenuation[DISTANCE].z;
}



SpotLight::SpotLight(vec3 pos, vec3 dir, vec3 col, float cut, float ocut, float a, float d, float s, Distance_ENUM dis) {
	color = col;
	this->a = a;
	this->d = d;
	this->s = s;

	position = pos;
	direction = normalize(dir);
	DISTANCE = dis;

	AcutOff = cut;
	AouterCutOff = ocut;

	updateLight(position, direction);
}
void SpotLight::setSpotLight(Shader shader) {
	shader.setVec3("spotlight.position",	&position);
	shader.setVec3("spotlight.direction",	&direction);
	shader.setVec3("spotlight.ambient",		&ambient);
	shader.setVec3("spotlight.diffuse",		&diffuse);
	shader.setVec3("spotlight.specular",	&specular);
	shader.setFloat("spotlight.cutOff",		cutOff);
	shader.setFloat("spotlight.outerCutOff",outerCutOff);
	shader.setFloat("spotlight.constant",	constant);
	shader.setFloat("spotlight.linear",		linear);
	shader.setFloat("spotlight.quadratic",	quadratic);
}
void SpotLight::updateLight(vec3 pos, vec3 dir) {
	ambient = a * color;
	diffuse = d * color;
	specular = s * color;

	position = pos;
	direction = dir;

	cutOff = cos(radians(AcutOff));
	outerCutOff = cos(radians(AouterCutOff));

	constant = lightAttenuation[DISTANCE].x;
	linear = lightAttenuation[DISTANCE].y;
	quadratic = lightAttenuation[DISTANCE].z;
}