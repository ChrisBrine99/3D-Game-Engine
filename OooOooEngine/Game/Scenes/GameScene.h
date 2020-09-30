#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "../../Engine/Core/CoreEngine.h"

class GameScene : public Scene {
private:
public:
	GameScene();
	virtual ~GameScene();

	virtual bool OnCreate();
	virtual void Update(const float deltaTime_);
	virtual void Render();
};

#endif