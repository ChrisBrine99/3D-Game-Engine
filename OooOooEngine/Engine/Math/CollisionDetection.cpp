#include "CollisionDetection.h"
#include "../Core/CoreEngine.h"
#include "Ray.h"

CollisionDetection::~CollisionDetection() {
}

Ray CollisionDetection::ScreenToWorldRay(glm::vec2 mousePos_, glm::vec2 screenSize_) {
	glm::vec4 startPoint_NDC((mousePos_.x / screenSize_.x - 0.5f) * 2.0f, (mousePos_.y / screenSize_.y - 0.5) * 2.0f, -1.0f, 1.0f);
	glm::vec4 endPoint_NDC(startPoint_NDC.x, startPoint_NDC.y, 0.0f, 1.0f);
	glm::mat4 inverse = glm::inverse(CoreEngine::GetInstance()->GetCamera()->GetPerspective() * CoreEngine::GetInstance()->GetCamera()->GetView());

	// Converting start and end point from NDC space to world space
	glm::vec4 startPoint_World = inverse * startPoint_NDC;
	glm::vec4 endPoint_World = inverse * endPoint_NDC;

	// Converting those two vec4 variables to vec3
	startPoint_World /= startPoint_World.w;
	endPoint_World /= endPoint_World.w;

	// Getting the direction of the ray in world space
	glm::vec3 rayDirection(endPoint_World - startPoint_World);
	rayDirection = glm::normalize(rayDirection);

	return Ray(glm::vec3(startPoint_World), rayDirection);
}

bool CollisionDetection::RayOBBIntersection(Ray* ray_, BoundingBox* box_) {
	float tMin = CoreEngine::GetInstance()->GetCamera()->GetNearPlane();
	float tMax = CoreEngine::GetInstance()->GetCamera()->GetFarPlane();
	glm::vec3 boxPosition(box_->transform[3].x, box_->transform[3].y, box_->transform[3].z);
	glm::vec3 delta = boxPosition - ray_->origin;

	// X-axis Plane
	glm::vec3 xPlane(box_->transform[0].x, box_->transform[0].y, box_->transform[0].z);
	float deltaProj = glm::dot(xPlane, delta);
	float rayProj = glm::dot(ray_->direction, xPlane);

	if (fabs(rayProj) > 0.001f) {
		float t1 = (deltaProj + box_->minVert.x) / rayProj;
		float t2 = (deltaProj + box_->maxVert.x) / rayProj;

		if (t1 > t2) { // Swap the values if they are flipped
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t2 < tMax) {
			tMax = t2;
		}

		if (t1 > tMin) {
			tMin = t1;
		}

		if (tMax < tMin) { // No collision detected
			return false;
		}
	} else { // Unique case: The ray is nearly parallel to the plane
		if (-deltaProj + box_->minVert.x > 0.0f || -deltaProj + box_->maxVert.x < 0.0f) {
			return false;
		}
	}

	// Y-axis Plane
	glm::vec3 yPlane(box_->transform[1].x, box_->transform[1].y, box_->transform[1].z);
	deltaProj = glm::dot(yPlane, delta);
	rayProj = glm::dot(ray_->direction, yPlane);

	if (fabs(rayProj) > 0.001f) {
		float t1 = (deltaProj + box_->minVert.y) / rayProj;
		float t2 = (deltaProj + box_->maxVert.y) / rayProj;

		if (t1 > t2) { // Swap the values if they are flipped
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t2 < tMax) {
			tMax = t2;
		}

		if (t1 > tMin) {
			tMin = t1;
		}

		if (tMax < tMin) { // No collision detected
			return false;
		}
	}
	else { // Unique case: The ray is nearly parallel to the plane
		if (-deltaProj + box_->minVert.y > 0.0f || -deltaProj + box_->maxVert.y < 0.0f) {
			return false;
		}
	}

	// Z-axis Plane
	glm::vec3 zPlane(box_->transform[2].x, box_->transform[2].y, box_->transform[2].z);
	deltaProj = glm::dot(zPlane, delta);
	rayProj = glm::dot(ray_->direction, zPlane);

	if (fabs(rayProj) > 0.001f) {
		float t1 = (deltaProj + box_->minVert.z) / rayProj;
		float t2 = (deltaProj + box_->maxVert.z) / rayProj;

		if (t1 > t2) { // Swap the values if they are flipped
			float temp = t1;
			t1 = t2;
			t2 = temp;
		}

		if (t2 < tMax) {
			tMax = t2;
		}

		if (t1 > tMin) {
			tMin = t1;
		}

		if (tMax < tMin) { // No collision detected
			return false;
		}
	}
	else { // Unique case: The ray is nearly parallel to the plane
		if (-deltaProj + box_->minVert.z > 0.0f || -deltaProj + box_->maxVert.z < 0.0f) {
			return false;
		}
	}

	// An intersection has occurred
	ray_->intersectDist = tMin;
	return true;
}

bool CollisionDetection::OBBInFrustum(glm::mat4 m_) {
	glm::vec3 position = glm::normalize(glm::vec3(m_[3].x, m_[3].y, m_[3].z));
	for (int i = 0; i < 6; i++) {
		
	}
	return true;
}