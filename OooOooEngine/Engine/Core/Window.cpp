#include "Window.h"

Window::Window() {
	width = 0;
	height = 0;
	window = nullptr;
	context = 0;
}

Window::~Window() {
	OnDestroy();
}

bool Window::OnCreate(std::string name_, int width_, int height_) {
	// Attempt to initialize SDL framework
	if (SDL_Init(SDL_INIT_VIDEO) < 0) { // SDL couldn't initialize
		DebugLogger::FatalError("SDL could not be initialized...", "Window.cpp", __LINE__);
		return false;
	}
	width = width_;
	height = height_;

	// Setting up necessities for before the window's creation
	SetPreAttributes();
	// Attempt to create SDL Window
	window = SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if (!window) { // Window couldn't be created
		DebugLogger::Error("Window failed to be created...", "Window.cpp", __LINE__);
		return false;
	}

	// Set the SDL GL context to the current window
	context = SDL_GL_CreateContext(window);

	// Setting up anything after the window is sucessfully created
	SetPostAttributes();

	// Attempt to initialize GLEW API
	GLenum error = glewInit();
	if (error != GLEW_OK) { // Glew failed to initialize
		DebugLogger::FatalError("Glew was unable to initialize...", "Window.cpp", __LINE__);
		return false;
	}
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);

	DebugLogger::Info("GPU -- " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))), "Window.cpp", __LINE__);
	DebugLogger::Info("OpenGL Version -- " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))), "Window.cpp", __LINE__);

	return true;
}

void Window::OnDestroy() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	window = nullptr;
}

int Window::GetWidth() {
	return width;
}

int Window::GetHeight() {
	return height;
}

SDL_Window* Window::GetWindow() const {
	return window;
}

void Window::SetPreAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// What version of openGL that is being used (EX. Major = X, Minor = Y, OpenGL Version = X.Y)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	// Enables double buffering on the GPU (If the GPU has time to render another frame while displaying a frame, it will do so)
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// Enables the GPU to sync drawing frames with the monitor's refresh rate
	SDL_GL_SetSwapInterval(1);
	// Allows the GPU to error check experimental GPU features if they are used
	glewExperimental = GL_TRUE;
}

void Window::SetPostAttributes() {
	// Sets what color depth to use (8-bit, 16-bit, 24-bit, 32-bit, etc.)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
}
