#include "GameScene.h"
#include "../../Engine/Audio/AudioHandler.h"
#include "../../Engine/Component/GUIImageComponent.h"
#include "../../Engine/Component/AudioSourceComponent.h"

GameScene::GameScene() {}
GameScene::~GameScene() {}

bool GameScene::OnCreate() {
	DebugLogger::Info("Creating Game Scene", "GameScene.cpp", __LINE__);

	CoreEngine::GetInstance()->SetCamera(new Camera);
	CoreEngine::GetInstance()->GetCamera()->SetPosition(glm::vec3(0.0f, 1.0f, 7.5f));
	CoreEngine::GetInstance()->GetCamera()->AddLightSource(new LightSource(glm::vec3(2.0f, 0.0f, 2.0f), glm::vec3(1.0f, 1.0f, 0.0f), 0.1f, glm::vec3(1.0f, 0.01f, 0.002f)));
	CollisionHandler::GetInstance()->OnCreate();

	AudioHandler::GetInstance()->OnCreate();

	Model* apple = new Model("Resources/Models/Apple.obj", "Resources/Materials/Apple.mtl", ShaderHandler::GetInstance()->GetShader("BasicShader"));
	SceneGraph::GetInstance()->AddModel(apple);

	Model* dice = new Model("Resources/Models/Dice.obj", "Resources/Materials/Dice.mtl", ShaderHandler::GetInstance()->GetShader("BasicShader"));
	SceneGraph::GetInstance()->AddModel(dice);

	SceneGraph::GetInstance()->AddGameObject(new GameObject(dice, glm::vec3(2.5f, 0.0f, 1.0f)));
	SceneGraph::GetInstance()->AddGameObject(new GameObject(dice, glm::vec3(-2.5f, 0.0f, 1.0f)), "Dice1");

	GameObject* shape = new GameObject(apple, glm::vec3(0.0f, 0.0f, 1.0f));
	SceneGraph::GetInstance()->AddGameObject(shape, "Apple1");
	//shape->AddComponent<TestComponent>();

	shape->AddComponent<AudioSourceComponent>("Resources/Audio/MeepMeep.wav", "Test");
	shape->SetScale(glm::vec3(0.5f));

	GUIObject* gui = new GUIObject(glm::vec2(0.0f));
	gui->AddComponent<GUIImageComponent>();

	TextureHandler::GetInstance()->CreateTexture("test", "Resources/Textures/CheckerboardTexture.png");
	gui->GetComponent<GUIImageComponent>()->OnCreate("test", glm::vec2(500.0f, 50.0f), glm::vec2(0.05f));
	SceneGraph::GetInstance()->AddGUIObject(gui);

	return true;
}

void GameScene::Update(const float deltaTime_) {
	SceneGraph::GetInstance()->Update(deltaTime_);
}

void GameScene::Render() {
	SceneGraph::GetInstance()->Render(CoreEngine::GetInstance()->GetCamera());
}

void GameScene::Draw() {
	SceneGraph::GetInstance()->Draw(CoreEngine::GetInstance()->GetCamera());
}