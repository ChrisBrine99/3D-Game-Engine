#include "Game1.h"

Game1::Game1() {
	curSceneNum = 0;
	curScene = nullptr;
}

Game1::~Game1() {
	delete curScene, curScene = nullptr;
}

bool Game1::OnCreate() {
	if (CoreEngine::GetInstance()->GetCurrentScene() == 0) {
		curScene = new StartScene();
		curSceneNum = 0;
		return curScene->OnCreate();
	}
	DebugLogger::FatalError("Engine's scene is not initialized to 0", "Game1.cpp", __LINE__);
	return false;
}

void Game1::Update(const float deltaTime) {
	if (curSceneNum != CoreEngine::GetInstance()->GetCurrentScene()) {
		BuildScene();
	}
	curScene->Update(deltaTime);
}

void Game1::Render() {
	curScene->Render();
}

void Game1::Draw() {
	curScene->Draw();
}

void Game1::BuildScene() {
	delete curScene, curScene = nullptr;
	// Load in the new scene
	switch (CoreEngine::GetInstance()->GetCurrentScene()) {
		case 1:
			curScene = new GameScene();
			break;
		default:
			curScene = new StartScene();
			break;
	}
	curSceneNum = CoreEngine::GetInstance()->GetCurrentScene();
	if (!curScene->OnCreate()) { // If the new scene couldn't be initialized
		CoreEngine::GetInstance()->Exit();
	}
}