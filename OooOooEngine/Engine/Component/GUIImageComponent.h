#ifndef GUI_IMAGE_COMPONENT_H
#define GUI_IMAGE_COMPONENT_H

#include "GUIComponent.h"
#include "../GUI/SpriteSurface.h"

class GUIImageComponent : public GUIComponent {
	SpriteSurface* sprite;
	glm::vec2 offset;
public:
	GUIImageComponent();
	virtual ~GUIImageComponent() final;

	void OnCreate(const std::string& _texture, glm::vec2 _offset = glm::vec2(0.0f), glm::vec2 _scale = glm::vec2(1.0f), float _angle = 0.0f, glm::vec4 _tint = glm::vec4(1.0f));

	virtual void Draw(Camera* _camera, glm::vec2 _position) override;
	virtual bool FindContainingPoint(glm::vec2 _position, glm::vec2 _point) override;
};

#endif