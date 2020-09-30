#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <glm/glm.hpp>

struct BoundingBox {
	glm::vec3 maxVert;
	glm::vec3 minVert;
	glm::mat4 transform;

	inline BoundingBox() {
		maxVert = minVert = glm::vec3();
		transform = glm::mat4();
	}

	inline BoundingBox(glm::vec3 maxVert_, glm::vec3 minVert_, glm::mat4 transform_) {
		maxVert = maxVert_;
		minVert = minVert_;
		transform = transform_;
	}

	inline bool IntersectsBox(BoundingBox* box_) {
		glm::vec3 minCorner = GetTransformedPoint(minVert, transform);
		glm::vec3 maxCorner = GetTransformedPoint(maxVert, transform);

		glm::vec3 oMinCorner = GetTransformedPoint(box_->minVert, box_->transform);
		glm::vec3 oMaxCorner = GetTransformedPoint(box_->maxVert, box_->transform);

		if (maxCorner.x < oMinCorner.x || minCorner.x > oMaxCorner.x) {
			return false;
		} else if (maxCorner.y < oMinCorner.y || minCorner.y > oMaxCorner.y) {
			return false;
		} else if (maxCorner.z < oMinCorner.z || minCorner.z > oMaxCorner.z) {
			return false;
		}

		return true;
	}

private:
	inline glm::vec3 GetTransformedPoint(glm::vec3 point_, glm::mat4 transform_) {
		return glm::vec3(transform_[3].x, transform_[3].y, transform_[3].z) + point_;
	}
};

#endif