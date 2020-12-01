#include "GUIObject.h"

GUIObject::GUIObject(glm::vec2 _position) :
	tag("") {
	position = _position;
}

GUIObject::~GUIObject() {
	for (auto c : components) {
		delete c, c = nullptr;
	}
	components.clear();
}

void GUIObject::Draw(Camera* _camera) {
	for (auto c : components) {
		c->Draw(_camera, position);
	}
}

bool GUIObject::ContainsPoint(glm::vec2 _point) {
	for (auto c : components) {
		if (c->FindContainingPoint(position, _point)) {
			return true;
		}
	}
	return false;
}