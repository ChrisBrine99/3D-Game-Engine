#ifndef CORE_ENGINE_H
#define CORE_ENGINE_H

#include "Window.h"
#include "Timer.h"
#include "DebugLogger.h"
#include "GameInterface.h"
#include "Scene.h"
#include "../Rendering/3D/GameObject.h"
#include "../Graphics/ShaderHandler.h"
#include "../Graphics/TextureHandler.h"
#include "../Camera/Camera.h"
#include "../Event/EventListener.h"
#include "../Rendering/SceneGraph.h"
#include <memory>

class CoreEngine {
private:
	Window* window;
	bool isRunning;

	Camera* camera;

	Timer timer;
	unsigned int FPS;

	GameInterface* gameInterface;
	int curSceneNum;

	static std::unique_ptr<CoreEngine> instance;
	friend std::default_delete<CoreEngine>;
public:
	CoreEngine(const CoreEngine&) = delete;
	CoreEngine(CoreEngine&&) = delete;
	CoreEngine& operator=(const CoreEngine&) = delete;
	CoreEngine& operator=(CoreEngine&&) = delete;

	CoreEngine();
	~CoreEngine();

	bool OnCreate(std::string name_, int width_, int height_);
	void OnDestroy();

	void Run();
	bool GetIsRunning() const;
	static CoreEngine* GetInstance();
	void SetGameInterface(GameInterface* gameInterface_);

	Camera* GetCamera() const;
	void SetCamera(Camera* camera_);

	int GetCurrentScene();
	void SetCurrentScene(int newScene_);

	void MousePressed(glm::vec2 mouse_, int button_);
	void MouseReleased(glm::vec2 mouse_, int button_);
	void MouseMoved(glm::vec2 mouse_);
	void MouseWheelMoved(int y_);

	glm::vec2 GetWindowSize();

	void Exit();
private:
	void Update(const float deltaTime);
	void Render();
};

#endif