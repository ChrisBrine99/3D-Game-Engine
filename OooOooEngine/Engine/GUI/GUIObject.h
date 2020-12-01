#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H

#include <vector>
#include <string>
#include "../Camera/Camera.h"
#include "../Component/GUIComponent.h"
#include "../Core/DebugLogger.h"

class GUIObject {
	std::vector<GUIComponent*> components;
	glm::vec2 position;
	std::string tag;
public:
	GUIObject(glm::vec2 _position);
	~GUIObject();

	void Draw(Camera* _camera);
	bool ContainsPoint(glm::vec2 _point);

	void SetTag(const std::string _tag) { tag = _tag; }
	std::string GetTag() const { return tag; }

	template<typename T> void AddComponent() {
		// Verifying that this GUIComponent isn't being added twice to the same object
		if (T* t = GetComponent<T>()) {
			DebugLogger::Warning("This component already exists on the GUI object!", "GUIObject.h", __LINE__);
			return;
		}
		// Ensuring that the component being added is actually a child of GUIComponent
		GUIComponent* component = dynamic_cast<GUIComponent*>(new T());
		if (component == nullptr) {
			DebugLogger::Error("Object attempting to be added isn't a child of the Component.h parent class!", "GUIObject.h", __LINE__);
			delete component, component = nullptr;
			return;
		}
		// If the component passes both checks, add it to the vector
		components.push_back(component);
	}

	template<typename T> void RemoveComponent() {
		for (auto it = components.begin(); it != components.end(); it++) {
			if (T* comp = dynamic_cast<T*>(*it)) {
				delete comp, comp = nullptr;
				components.erase(it);
				return true;
			}
		}
		DebugLogger::Warning("That component doesn't exist on the current game object!", "GUIObject.h", __LINE__);
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