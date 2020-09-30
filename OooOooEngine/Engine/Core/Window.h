#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <glew.h>
#include <SDL_opengl.h>
#include <string>
#include "DebugLogger.h"

class Window {
private:
	SDL_Window* window;
	SDL_GLContext context;
	int width, height;
public:
	Window();
	~Window();

	bool OnCreate(std::string name_, int width_, int height_);
	void OnDestroy();

	int GetWidth();
	int GetHeight();
	SDL_Window* GetWindow() const;
private:
	void SetPreAttributes();
	void SetPostAttributes();
};

#endif