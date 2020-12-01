#include "GUIImageComponent.h"

GUIImageComponent::GUIImageComponent() :
	sprite(nullptr), offset(0.0f) {

}

GUIImageComponent::~GUIImageComponent() {
	if (sprite) { delete sprite, sprite = nullptr; }
}

void GUIImageComponent::OnCreate(const std::string& _texture, glm::vec2 _offset, glm::vec2 _scale, float _angle, glm::vec4 _tint) {
	offset = _offset;
	if (!sprite) {
		sprite = new SpriteSurface(_texture, _scale, _angle, _tint);
	}
}

void GUIImageComponent::Draw(Camera* _camera, glm::vec2 _position) {
	sprite->Draw(_camera, _position + offset);
}

bool GUIImageComponent::FindContainingPoint(glm::vec2 _position, glm::vec2 _point) {
	if (!sprite) {
		return false;
	}

	glm::vec2 min = _position + offset - sprite->GetScale();
	glm::vec2 max = _position + offset + sprite->GetScale();

	if (_point.x >= min.x && _point.x <= max.x && _point.y >= min.y && _point.y <= max.y) {
		return true;
	}

	return false;
}