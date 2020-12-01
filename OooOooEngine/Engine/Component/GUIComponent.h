#ifndef GUI_COMPONENT_H
#define GUI_COMPONENT_H

#include <glm/glm.hpp>
#include "../Camera/Camera.h"

class GUIComponent {
public:
	GUIComponent() {};
	virtual ~GUIComponent() = 0 {};

	virtual void Draw(Camera* _camera, glm::vec2 _position) = 0;
	virtual bool FindContainingPoint(glm::vec2 _position, glm::vec2 _point) = 0;
};

#endif