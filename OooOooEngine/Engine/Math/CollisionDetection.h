#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include <glm/glm.hpp>

struct BoundingBox;
struct Ray;
class CollisionDetection {
public:
	CollisionDetection(const CollisionDetection&) = delete;
	CollisionDetection(CollisionDetection&&) = delete;
	CollisionDetection& operator=(const CollisionDetection&) = delete;
	CollisionDetection& operator=(CollisionDetection&&) = delete;

	CollisionDetection() = delete;
	~CollisionDetection();

	static Ray ScreenToWorldRay(glm::vec2 mousePos_, glm::vec2 screenSize_);
	static bool RayOBBIntersection(Ray* ray_, BoundingBox* box_);
	static bool OBBInFrustum(glm::mat4 m_);
};

#endif