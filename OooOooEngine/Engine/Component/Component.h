#ifndef COMPONENT_H
#define COMPONENT_H

class GameObject;
class Component {
protected:
	GameObject* parent;
public:
	Component() : parent(nullptr) {};
	virtual ~Component() = 0 { parent = nullptr; };

	virtual bool OnCreate(GameObject* _gameObject) = 0;
	virtual void Update(float _deltaTime) = 0;
};

#endif