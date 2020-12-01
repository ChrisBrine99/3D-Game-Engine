#ifndef GAME_INTERFACE_H
#define GAME_INTERFACE_H

class GameInterface {
public:
	GameInterface() {}
	virtual ~GameInterface() {}

	virtual bool OnCreate() = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void Draw() = 0;
};

#endif