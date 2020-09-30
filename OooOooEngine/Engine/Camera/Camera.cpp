#include "Camera.h"
#include "../Core/CoreEngine.h"

glm::vec4 Camera::frustumData[6];

Camera::Camera() {
	position = glm::vec3(0.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	localUp = glm::vec3(0.0f, 1.0f, 0.0f);
	worldUp = localUp;
	fov = 45.0f;
	nearPlane = 0.2f;
	farPlane = 10000.0f;
	yaw = -90.0f;
	pitch = 0.0f;
	glm::vec2 windowSize = CoreEngine::GetInstance()->GetWindowSize();
	perspectiveMat = glm::perspective(fov, windowSize.x / windowSize.y, nearPlane, farPlane);
	orthographicMat = glm::ortho(0.0f, windowSize.x, 1.0f, windowSize.y, -1.0f, 1.0f);

	lights = std::vector<LightSource*>();
	lights.reserve(5);
}

Camera::~Camera() {
	OnDestroy();
}

void Camera::OnDestroy() {
	if (lights.size() > 0) {
		for (auto l : lights) {
			delete l, l = nullptr;
		}
		lights.clear();
	}
}

void Camera::ProcessMouseMovement(glm::vec2 offset_) {
	offset_ *= 0.05;

	yaw += offset_.x;
	pitch -= offset_.y;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	} else if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	if (yaw > 360.0f) {
		yaw -= 360.0f;
	} else if (yaw < 0.0f) {
		yaw += 360.0f;
	}

	UpdateCameraVector();
}

void Camera::ProcessMouseZoom(int y_) {
	if (y_ != 0) { // Make sure the mouse wheel is actually being scrolled
		position += static_cast<float>(y_) * forward * 2.0f;
	}
}

void Camera::SetPosition(glm::vec3 position_) {
	position = position_;
	UpdateCameraVector();
}

void Camera::SetRotation(float yaw_, float pitch_) {
	yaw = yaw_;
	pitch = pitch;
	UpdateCameraVector();
}

glm::vec3 Camera::GetPosition() {
	return position;
}

glm::mat4 Camera::GetView() const {
	return lookAt(position, position + forward, localUp);
}

glm::mat4 Camera::GetPerspective() const {
	return perspectiveMat;
}

glm::mat4 Camera::GetOrthographic() const {
	return orthographicMat;
}

float Camera::GetNearPlane() const {
	return nearPlane;
}

float Camera::GetFarPlane() const {
	return farPlane;
}

void Camera::AddLightSource(LightSource* newLight_) {
	lights.push_back(newLight_);
}

std::vector<LightSource*> Camera::GetLights() {
	return lights;
}

void Camera::CalculateFrustum() {
	// Getting the camera's current projection matrix
	float projection[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projection);
	// Getting openGL's model view matrix and storing it in a local variable
	float model[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, model);

	// Calculate the clipping planes
	float clip[16];

	clip[0] = model[0] * projection[0] + model[1] * projection[4] + model[2] * projection[8] + model[3] * projection[12];
	clip[1] = model[0] * projection[1] + model[1] * projection[5] + model[2] * projection[9] + model[3] * projection[13];
	clip[2] = model[0] * projection[2] + model[1] * projection[6] + model[2] * projection[10] + model[3] * projection[14];
	clip[3] = model[0] * projection[3] + model[1] * projection[7] + model[2] * projection[11] + model[3] * projection[15];

	clip[4] = model[4] * projection[0] + model[5] * projection[4] + model[6] * projection[8] + model[7] * projection[12];
	clip[5] = model[4] * projection[1] + model[5] * projection[5] + model[6] * projection[9] + model[7] * projection[13];
	clip[6] = model[4] * projection[2] + model[5] * projection[6] + model[6] * projection[10] + model[7] * projection[14];
	clip[7] = model[4] * projection[3] + model[5] * projection[7] + model[6] * projection[11] + model[7] * projection[15];

	clip[8] = model[8] * projection[0] + model[9] * projection[4] + model[10] * projection[8] + model[11] * projection[12];
	clip[9] = model[8] * projection[1] + model[9] * projection[5] + model[10] * projection[9] + model[11] * projection[13];
	clip[10] = model[8] * projection[2] + model[9] * projection[6] + model[10] * projection[10] + model[11] * projection[14];
	clip[11] = model[8] * projection[3] + model[9] * projection[7] + model[10] * projection[11] + model[11] * projection[15];

	clip[12] = model[12] * projection[0] + model[13] * projection[4] + model[14] * projection[8] + model[15] * projection[12];
	clip[13] = model[12] * projection[1] + model[13] * projection[5] + model[14] * projection[9] + model[15] * projection[13];
	clip[14] = model[12] * projection[2] + model[13] * projection[6] + model[14] * projection[10] + model[15] * projection[14];
	clip[15] = model[12] * projection[3] + model[13] * projection[7] + model[14] * projection[11] + model[15] * projection[15];

	// Getting the sides of the frustum from the clipping plane data
	frustumData[LEFT].x = clip[3] - clip[0];
	frustumData[LEFT].y = clip[7] - clip[4];
	frustumData[LEFT].z = clip[11] - clip[8];
	frustumData[LEFT].w = clip[15] - clip[12];

	frustumData[RIGHT].x = clip[3] + clip[0];
	frustumData[RIGHT].y = clip[7] + clip[4];
	frustumData[RIGHT].z = clip[11] + clip[8];
	frustumData[RIGHT].w = clip[15] + clip[12];

	frustumData[TOP].x = clip[3] + clip[1];
	frustumData[TOP].y = clip[7] + clip[5];
	frustumData[TOP].x = clip[11] + clip[9];
	frustumData[TOP].x = clip[15] + clip[13];

	frustumData[BOTTOM].x = clip[3] - clip[1];
	frustumData[BOTTOM].y = clip[7] - clip[5];
	frustumData[BOTTOM].x = clip[11] - clip[9];
	frustumData[BOTTOM].x = clip[15] - clip[13];
	
	frustumData[BACK].x = clip[3] - clip[2];
	frustumData[BACK].y = clip[7] - clip[6];
	frustumData[BACK].z = clip[11] - clip[10];
	frustumData[BACK].w = clip[15] - clip[14];

	frustumData[FRONT].x = clip[3] + clip[2];
	frustumData[FRONT].y = clip[7] + clip[6];
	frustumData[FRONT].z = clip[11] + clip[10];
	frustumData[FRONT].w = clip[15] + clip[14];
}

void Camera::UpdateCameraVector() {
	// Calculating the new forward vector
	forward.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	forward.y = glm::sin(glm::radians(pitch));
	forward.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	forward = glm::normalize(forward);
	// Next Step: Calculating the right vector
	right = glm::normalize(glm::cross(forward, worldUp));
	// Finally: Calculating the localUp vector
	localUp = glm::normalize(glm::cross(right, forward));
	// Recalculate the frustum
	CalculateFrustum();
}