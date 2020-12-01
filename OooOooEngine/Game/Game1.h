#ifndef GAME1_H
#define GAME1_H

#include "../Engine/Core/CoreEngine.h"
#include "Scenes/StartScene.h"
#include "Scenes/GameScene.h"

class Game1 : public GameInterface {
private:
	int curSceneNum;
	Scene* curScene;
public:
	Game1();
	virtual ~Game1();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime);
	virtual void Render();
	virtual void Draw();
private:
	void BuildScene();
};

#endif

