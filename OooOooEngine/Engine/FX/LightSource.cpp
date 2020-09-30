#include "LightSource.h"

LightSource::LightSource(glm::vec3 position_, glm::vec3 color_, float ambient_, glm::vec3 attenuation_) {
	position = position_;
	color = color_;
	ambient = ambient_;
	attenuation = attenuation_;
}

LightSource::~LightSource() {
}

void LightSource::SetPosition(glm::vec3 position_) {
	position = position_;
}

glm::vec3 LightSource::GetPosition() {
	return position;
}

void LightSource::SetColor(glm::vec3 color_) {
	color = color_;
}

glm::vec3 LightSource::GetColor() {
	return color;
}

void LightSource::SetAmbient(float ambient_) {
	ambient = ambient_;
}

float LightSource::GetAmbient() {
	return ambient;
}

void LightSource::SetAttenuation(glm::vec3 attenuation_) {
	attenuation = attenuation_;
}

glm::vec3 LightSource::GetAttenuation() {
	return attenuation;
}