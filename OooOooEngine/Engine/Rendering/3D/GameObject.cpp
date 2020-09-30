#include "GameObject.h"

GameObject::GameObject(Model* model_, glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scaleFactor_) {
	model = model_;
	position = position_;
	rotation = rotation_;
	angle = angle_;
	scaleFactor = scaleFactor_;
	modelInstance = -1;
	name = "";
	isHit = false;
	if (model) { // If the model exists; create its matrix
		modelInstance = model->CreateInstance(position, rotation, angle, scaleFactor);
		box = model->GetBoundingBox();
		box.transform = model->GetTransform(modelInstance);
	}
	// Add this object to the scene graph
}

GameObject::~GameObject() {
	model->RemoveInstance(modelInstance);
	model = nullptr;
}

void GameObject::Update(const float deltaTime_) {
	SetAngle(angle + 0.005f);
}

void GameObject::SetAngle(float angle_) {
	angle = angle_;
	if (model) { // If the model exists, update it to reflect the new angle
		model->UpdateInstance(modelInstance, position, rotation, angle, scaleFactor);
		box.transform = model->GetTransform(modelInstance);
	}
}

float GameObject::GetAngle() const {
	return angle;
}

void GameObject::SetPosition(glm::vec3 position_) {
	position = position_;
	if (model) { // If the model exists, update it to reflect the new position
		model->UpdateInstance(modelInstance, position, rotation, angle, scaleFactor);
		box.transform = model->GetTransform(modelInstance);
	}
}

glm::vec3 GameObject::GetPosition() const {
	return position;
}

void GameObject::SetRotation(glm::vec3 rotation_) {
	rotation = rotation_;
	if (model) { // If the model exists, update it to reflect the new rotation
		model->UpdateInstance(modelInstance, position, rotation, angle, scaleFactor);
		box.transform = model->GetTransform(modelInstance);
	}
}

glm::vec3 GameObject::GetRotation() const {
	return rotation;
}

void GameObject::SetScale(glm::vec3 scaleFactor_) {
	scaleFactor = scaleFactor_;
	if (model) { // If the model exists, update it to reflect the new scaling
		model->UpdateInstance(modelInstance, position, rotation, angle, scaleFactor);
		box.transform = model->GetTransform(modelInstance);
		box.minVert *= scaleFactor.x > 1.0f ? scaleFactor.x : (scaleFactor.x / 2);
		box.maxVert *= scaleFactor.x > 1.0f ? scaleFactor.x : (scaleFactor.x / 2);
	}
}

glm::vec3 GameObject::GetScale() const {
	return scaleFactor;
}

BoundingBox GameObject::GetBoundingBox() {
	return box;
}

void GameObject::SetName(std::string name_) {
	name = name_;
}

std::string GameObject::GetName() const {
	return name;
}

void GameObject::SetIsHit(bool isHit_, int button_) {
	isHit = isHit_;
	if (isHit) { // To check if collision actually worked or not
		std::cout << name << " was hit." << std::endl;
	}
}

bool GameObject::GetIsHit() const {
	return isHit;
}