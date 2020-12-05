#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../../Component/Component.h"
#include "Model.h"

class GameObject {
private:
	Model* model;
	int modelInstance;

	std::string name;
	float angle;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaleFactor;

	BoundingBox box;
	bool isHit;

	std::vector<Component*> components;
public:
	GameObject(Model* model_, glm::vec3 position_ = glm::vec3(), glm::vec3 rotation_ = glm::vec3(0.0f, 1.0f, 0.0f), float angle_ = 0.0f, glm::vec3 scaleFactor_ = glm::vec3(1.0f));
	~GameObject();

	void Update(const float deltaTime_);

	void SetAngle(float angle_);
	float GetAngle() const;

	void SetPosition(glm::vec3 position_);
	glm::vec3 GetPosition() const;

	void SetRotation(glm::vec3 rotation_);
	glm::vec3 GetRotation() const;

	void SetScale(glm::vec3 scaleFactor_);
	glm::vec3 GetScale() const;

	BoundingBox GetBoundingBox();

	void SetIsHit(bool isHit_, int button_);
	bool GetIsHit() const;

	void SetName(std::string name_);
	std::string GetName() const;

	template<class T, class... Args> void AddComponent(Args&&... args) {
		// Verifying that this Component isn't being added twice to the same object
		if (T* t = GetComponent<T>()) {
			DebugLogger::Warning("This component already exists on the game object!", "GameObject.h", __LINE__);
			t = nullptr;
			return;
		}
		// Ensuring that the component being added is actually a child of component
		T* comp = new T(std::forward<Args>(args)...);
		Component* component = dynamic_cast<Component*>(comp);
		if (component == nullptr) {
			DebugLogger::Error("Object attempting to be added isn't a child of the Component.h parent class!", "GameObject.h", __LINE__);
			delete comp, comp = nullptr;
			return;
		}
		// If the component passes both checks, add it to the vector
		components.push_back(component);
		component->OnCreate(this);
	}

	template<typename T> bool RemoveComponent() {
		for (auto it = components.begin(); it != components.end(); it++) {
			if (T* comp = dynamic_cast<T*>(*it)) {
				delete comp, comp = nullptr;
				components.erase(it);
				return true;
			}
		}
		DebugLogger::Warning("That component doesn't exist on the current game object!", "GameObject.h", __LINE__);
		return false;
	}

	template<typename T> T* GetComponent() {
		for (size_t i = 0; i < components.size(); i++) {
			if (T* comp = dynamic_cast<T*>(components[i])) {
				return comp;
			}
		}
		return nullptr;
	}
};

#endif