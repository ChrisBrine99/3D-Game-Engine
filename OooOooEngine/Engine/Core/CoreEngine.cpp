#include "CoreEngine.h"

std::unique_ptr<CoreEngine> CoreEngine::instance = nullptr;

CoreEngine::CoreEngine() {
	window = nullptr;
	camera = nullptr;
	isRunning = false;
	FPS = 60;
	curSceneNum = 0;
}

CoreEngine::~CoreEngine() {
	OnDestroy();
}

bool CoreEngine::OnCreate(std::string name_, int width_, int height_) {
	DebugLogger::DebugInit();
	DebugLogger::SetSeverity(MESSAGE_TYPE::INFO);
	// Create and attempt to initialize the window
	window = new Window();
	isRunning = window->OnCreate(name_.c_str(), width_, height_);
	if (!isRunning) { // Window failed to be initialized; return false
		DebugLogger::FatalError("Window could not be initialized...", "CoreEngine.cpp", __LINE__);
		return false;
	}

	SDL_WarpMouseInWindow(window->GetWindow(), width_ / 2, height_ / 2);
	MouseEventListener::RegisterEngineObject(this);

	ShaderHandler::GetInstance()->CreateProgram("ColorShader",		"Engine/Shaders/ColorVertexShader.glsl",		"Engine/Shaders/ColorFragmentShader.glsl");
	ShaderHandler::GetInstance()->CreateProgram("BasicShader",		"Engine/Shaders/VertexShader.glsl",				"Engine/Shaders/FragmentShader.glsl");
	ShaderHandler::GetInstance()->CreateProgram("SpriteShader",		"Engine/Shaders/SpriteVertShader.glsl",			"Engine/Shaders/SpriteFragShader.glsl");

	if (gameInterface) {
		if (!gameInterface->OnCreate()) { // Game Interface failed to initialize
			DebugLogger::FatalError("Game Interface has failed to be created.", "CoreEngine.cpp", __LINE__);
			return false;
		}
	}

	timer.Start();

	DebugLogger::Info("Everything has been initialized!", "CoreEngine.cpp", __LINE__);
	return true;
}

void CoreEngine::OnDestroy() {
	ShaderHandler::GetInstance()->OnDestroy();
	TextureHandler::GetInstance()->OnDestroy();
	SceneGraph::GetInstance()->OnDestroy();

	delete camera, camera = nullptr;
	delete gameInterface, gameInterface = nullptr;
	delete window, window = nullptr;

	SDL_Quit();
	exit(0);
}

void CoreEngine::Run() {
	while (isRunning) { // Main game loop
		timer.UpdateFrameTicks();
		// Check input
		EventListener::Update();
		// Update and render
		Update(timer.GetDeltaTime());
		Render();
		// Delay if necessary
		SDL_Delay(timer.GetSleepTime(FPS));
	}
	// Loop has ended; clean up and close
	OnDestroy();
}

bool CoreEngine::GetIsRunning() const {
	return isRunning;
}

CoreEngine* CoreEngine::GetInstance() {
	CoreEngine* instance_ = instance.get();
	if (instance_ == nullptr) {
		instance.reset(new CoreEngine());
		instance_ = instance.get();
	}
	return instance_;
}

void CoreEngine::SetGameInterface(GameInterface * gameInterface_) {
	gameInterface = gameInterface_;
}


Camera* CoreEngine::GetCamera() const {
	return camera;
}

void CoreEngine::SetCamera(Camera* camera_) {
	camera = camera_;
}

int CoreEngine::GetCurrentScene() {
	return curSceneNum;
}

void CoreEngine::SetCurrentScene(int newScene_) {
	curSceneNum = newScene_;
}

void CoreEngine::MousePressed(glm::vec2 mouse_, int button_) {
	//if (button_ == SDL_BUTTON_LEFT) { // Removes the apple game object from the scene
	//	SceneGraph::GetInstance()->RemoveGameObject("Apple1");
	//}
}

void CoreEngine::MouseReleased(glm::vec2 mouse_, int button_) {
	CollisionHandler::GetInstance()->MouseEvent(mouse_, button_);
}

void CoreEngine::MouseMoved(glm::vec2 mouse_) {
	if (camera) {
		camera->ProcessMouseMovement(MouseEventListener::GetMouseOffset());
	}
}

void CoreEngine::MouseWheelMoved(int y_) {
	if (camera) {
		camera->ProcessMouseZoom(y_);
	}
}

glm::vec2 CoreEngine::GetWindowSize() {
	return glm::vec2(window->GetWidth(), window->GetHeight());
}

void CoreEngine::Exit() {
	isRunning = false;
}

void CoreEngine::Update(const float deltaTime) {
	if (gameInterface) { // Update the Game
		gameInterface->Update(deltaTime);
	}
}

void CoreEngine::Render() {
	// Clear the screen for the frame
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (gameInterface) { // Render the Game and GUI
		gameInterface->Render();
		gameInterface->Draw();
	}

	// Swap the buffer frame
	SDL_GL_SwapWindow(window->GetWindow());
}
