#include "CollisionHandler.h"
#include "../Core/CoreEngine.h"

std::vector<GameObject*> CollisionHandler::colliders = std::vector<GameObject*>();
std::vector<GameObject*> CollisionHandler::prevCollisions = std::vector<GameObject*>();
std::unique_ptr<CollisionHandler> CollisionHandler::instance = nullptr;

CollisionHandler::CollisionHandler(float worldSize_) {
	colliders.reserve(10);
	prevCollisions.reserve(10);
	partition = new OctSpatialPartition(worldSize_, 3);
}

CollisionHandler::~CollisionHandler() {
	OnDestroy();
}

void CollisionHandler::OnCreate() {
	colliders.clear();
	prevCollisions.clear();
}

void CollisionHandler::OnDestroy() {
	if (colliders.size() > 0) {
		for (auto c : colliders) {
			c = nullptr;
		}
		colliders.clear();
	}

	if (prevCollisions.size() > 0) {
		for (auto p : prevCollisions) {
			p = nullptr;
		}
		prevCollisions.clear();
	}
}

void CollisionHandler::AddObject(GameObject* gameObject_) {
	colliders.push_back(gameObject_);
	partition->AddObject(gameObject_);
}

void CollisionHandler::RemoveObject(GameObject* gameObject_) {
	for (auto c : colliders) {
		if (c == gameObject_) {
			c = nullptr;
		}
	}
}

void CollisionHandler::MouseEvent(glm::vec2 mousePos_, int button_) {
	Ray mouseRay = CollisionDetection::ScreenToWorldRay(mousePos_, CoreEngine::GetInstance()->GetWindowSize());
	if (partition) {
		GameObject* hitResult = partition->GetCollidedObject(mouseRay);
		if (hitResult != nullptr) { // If an object was hit
			hitResult->SetIsHit(true, button_);
		}

		for (auto p : prevCollisions) {
			if (p != hitResult && p != nullptr) {
				p->SetIsHit(false, button_);
				p = nullptr;
			}
		}

		if (hitResult) {
			prevCollisions.push_back(hitResult);
		}
	}
}

CollisionHandler* CollisionHandler::GetInstance() {
	if (instance.get() == nullptr) {
		instance.reset(new CollisionHandler(100.0f));
	}
	return instance.get();
}