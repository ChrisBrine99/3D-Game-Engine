#include "MouseEventListener.h"
#include "../Core/CoreEngine.h"

CoreEngine* MouseEventListener::engineInstance = nullptr;

glm::vec2 MouseEventListener::mousePos = glm::vec2();
glm::vec2 MouseEventListener::prevMousePos = glm::vec2();
bool MouseEventListener::firstUpdate = true;

MouseEventListener::~MouseEventListener() {
	engineInstance = nullptr;
}

void MouseEventListener::RegisterEngineObject(CoreEngine* coreEngine_) {
	engineInstance = coreEngine_;
}

void MouseEventListener::Update(SDL_Event e_) { // Handling SDL input events
	switch (e_.type) {
		case SDL_MOUSEBUTTONDOWN: // A button on the mouse was pressed down
			UpdateMousePosition();
			NotifyMousePressed(e_.button.button);
			break;
		case SDL_MOUSEBUTTONUP: // A button on the mouse was released
			UpdateMousePosition();
			NotifyMouseReleased(e_.button.button);
			break;
		case SDL_MOUSEMOTION: // The mouse has been moved by the user
			UpdateMousePosition();
			NotifyMouseMoved();
			break;
		case SDL_MOUSEWHEEL: // The user has scrolled the mouse wheel in a direction
			NotifyMouseScrolled(e_.wheel.y);
			break;
	}
}

void MouseEventListener::NotifyMousePressed(int buttonType_) {
	if (engineInstance) { // Makes sure the engine has been registered
		engineInstance->MousePressed(mousePos, buttonType_);
	}
}

void MouseEventListener::NotifyMouseReleased(int buttonType_) {
	if (engineInstance) { // Makes sure the engine has been registered
		engineInstance->MouseReleased(mousePos, buttonType_);
	}
}

void MouseEventListener::NotifyMouseMoved() {
	if (engineInstance) { // Make sure the engine has been registered
		engineInstance->MouseMoved(mousePos);
	}
}

void MouseEventListener::NotifyMouseScrolled(int y_) {
	if (engineInstance) { // Make sure the engine has been registered
		engineInstance->MouseWheelMoved(y_);
	}
}

glm::vec2 MouseEventListener::GetPreviousMousePosition() {
	return prevMousePos;
}

glm::vec2 MouseEventListener::GetMousePosition() {
	return mousePos;
}

glm::vec2 MouseEventListener::GetMouseOffset() {
	return glm::vec2(mousePos.x - prevMousePos.x, prevMousePos.y - mousePos.y);
}

void MouseEventListener::UpdateMousePosition() {
	glm::ivec2 tempMousePos;
	SDL_GetMouseState(&tempMousePos.x, &tempMousePos.y);
	tempMousePos.y = engineInstance->GetInstance()->GetWindowSize().y - tempMousePos.y;
	if (firstUpdate) { // Previous mouse position and current mouse position should be the same on the first update
		prevMousePos.x = mousePos.x = tempMousePos.x;
		prevMousePos.y = mousePos.y = tempMousePos.y;
		firstUpdate = false;
	} else if (tempMousePos.x != prevMousePos.x || tempMousePos.y != prevMousePos.y) {
		prevMousePos.x = mousePos.x;
		prevMousePos.y = mousePos.y;
		mousePos.x = tempMousePos.x;
		mousePos.y = tempMousePos.y;
	}
}