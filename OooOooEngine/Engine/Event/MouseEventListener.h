#ifndef MOUSE_EVENT_LISTENER_H
#define MOUSE_EVENT_LISTENER_H

#include <SDL.h>
#include <glm/glm.hpp>

class CoreEngine;
class MouseEventListener {
private:
	static CoreEngine* engineInstance;

	static glm::vec2 mousePos, prevMousePos;
	static bool firstUpdate;
public:
	MouseEventListener(const MouseEventListener&) = delete;
	MouseEventListener(MouseEventListener&&) = delete;
	MouseEventListener& operator=(const MouseEventListener&) = delete;
	MouseEventListener& operator=(MouseEventListener&&) = delete;

	MouseEventListener() = delete;
	~MouseEventListener();

	static void RegisterEngineObject(CoreEngine* coreEngine_);
	static void Update(SDL_Event e_);

	static void NotifyMousePressed(int buttonType_);
	static void NotifyMouseReleased(int buttonType_);
	static void NotifyMouseMoved();
	static void NotifyMouseScrolled(int y_);

	static glm::vec2 GetPreviousMousePosition();
	static glm::vec2 GetMousePosition();
	static glm::vec2 GetMouseOffset();
private:
	static void UpdateMousePosition();
};

#endif