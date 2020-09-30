#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

class GameObject {
private:
	Model* model;
	int modelInstance;

	std::string name;
	float angle;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scaleFactor;

	BoundingBox box;
	bool isHit;
public:
	GameObject(Model* model_, glm::vec3 position_ = glm::vec3(), glm::vec3 rotation_ = glm::vec3(0.0f, 1.0f, 0.0f), float angle_ = 0.0f, glm::vec3 scaleFactor_ = glm::vec3(1.0f));
	~GameObject();

	void Update(const float deltaTime_);

	void SetAngle(float angle_);
	float GetAngle() const;

	void SetPosition(glm::vec3 position_);
	glm::vec3 GetPosition() const;

	void SetRotation(glm::vec3 rotation_);
	glm::vec3 GetRotation() const;

	void SetScale(glm::vec3 scaleFactor_);
	glm::vec3 GetScale() const;

	BoundingBox GetBoundingBox();

	void SetIsHit(bool isHit_, int button_);
	bool GetIsHit() const;

	void SetName(std::string name_);
	std::string GetName() const;
};

#endif