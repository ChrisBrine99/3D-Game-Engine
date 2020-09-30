#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include "../Rendering/3D/GameObject.h"
#include "../Core/OctSpatialPartition.h"

class CollisionHandler {
private:
	static std::vector<GameObject*> colliders;
	static std::vector<GameObject*> prevCollisions;
	OctSpatialPartition* partition;

	static std::unique_ptr<CollisionHandler> instance;
	friend std::default_delete<CollisionHandler>;
public:
	CollisionHandler(const CollisionHandler&) = delete;
	CollisionHandler(CollisionHandler&&) = delete;
	CollisionHandler& operator=(const CollisionHandler&) = delete;
	CollisionHandler& operator=(CollisionHandler&&) = delete;

	CollisionHandler(float worldSize_);
	~CollisionHandler();
	void OnCreate();
	void OnDestroy();

	void AddObject(GameObject* gameObject_);
	void RemoveObject(GameObject* gameObject_);

	void MouseEvent(glm::vec2 mousePos_, int button_);

	static CollisionHandler* GetInstance();
};

#endif