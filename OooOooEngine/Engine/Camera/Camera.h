#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../FX/LightSource.h"

enum FRUSTUM {
	RIGHT = 0,
	LEFT = 1,
	FRONT = 2,
	BACK = 3,
	TOP = 4,
	BOTTOM = 5
};

class Camera {
public:
	static glm::vec4 frustumData[6];	// 6 for the six frustum sides
private:
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 localUp;
	glm::vec3 right;
	glm::vec3 worldUp;

	glm::mat4 perspectiveMat;
	glm::mat4 orthographicMat;

	float nearPlane;
	float farPlane;

	float fov;
	float yaw;
	float pitch;

	std::vector<LightSource*> lights;
public:
	Camera();
	~Camera();

	void OnDestroy();

	void ProcessMouseMovement(glm::vec2 offset_);
	void ProcessMouseZoom(int y_);

	void SetPosition(glm::vec3 position_);
	void SetRotation(float yaw_, float pitch_);
	glm::vec3 GetPosition();

	glm::mat4 GetView() const;
	glm::mat4 GetPerspective() const;
	glm::mat4 GetOrthographic() const;

	float GetNearPlane() const;
	float GetFarPlane() const;

	void AddLightSource(LightSource* newLight_);
	std::vector<LightSource*> GetLights();

	void CalculateFrustum();
private:
	void UpdateCameraVector();
};

#endif