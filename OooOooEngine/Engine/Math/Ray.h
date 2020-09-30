#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "BoundingBox.h"
#include "CollisionDetection.h"

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;
	float intersectDist;

	inline Ray() {
		origin = direction = glm::vec3();
		intersectDist = 0.0f;
	}

	inline Ray(glm::vec3 origin_, glm::vec3 direction_) {
		origin = origin_;
		direction = direction_;
		intersectDist = 0.0f;
	}

	inline Ray& operator = (const Ray& ray_) {
		origin = ray_.origin;
		direction = ray_.direction;
		intersectDist = 0.0f;
	}

	inline bool IsColliding(BoundingBox* box_) {
		intersectDist = -1.0f;
		return CollisionDetection::RayOBBIntersection(this, box_);
	}
};

#endif