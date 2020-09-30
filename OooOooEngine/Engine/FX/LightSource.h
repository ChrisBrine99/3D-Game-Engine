#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#define MAX_LIGHTS 4
#include <glm/glm.hpp>

class LightSource {
private:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 attenuation;
	float ambient;
public:
	LightSource(glm::vec3 position_, glm::vec3 color_, float ambient_, glm::vec3 attenuation_ = glm::vec3(1.0f, 0.0f, 0.0f));
	~LightSource();

	void SetPosition(glm::vec3 position_);
	glm::vec3 GetPosition();

	void SetColor(glm::vec3 color_);
	glm::vec3 GetColor();

	void SetAmbient(float ambient_);
	float GetAmbient();

	void SetAttenuation(glm::vec3 attenuation_);
	glm::vec3 GetAttenuation();
};

#endif