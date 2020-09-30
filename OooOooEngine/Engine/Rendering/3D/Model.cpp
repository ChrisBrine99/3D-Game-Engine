#include "Model.h"
#include "../SceneGraph.h"

Model::Model(const std::string &objFilePath_, const std::string &matFilePath_, GLuint shaderProgram_) {
	totalInstances = 0;
	shaderProgram = shaderProgram_;

	subMeshes = std::vector<Mesh*>();
	subMeshes.reserve(5);
	modelInstances = std::vector<glm::mat4>();
	modelInstances.reserve(5);

	LoadModel(objFilePath_, matFilePath_);
}

Model::~Model() {
	if (subMeshes.size() > 0) { // If any meshes exist; destroy them
		for (auto m : subMeshes) {
			delete m, m = nullptr;
		}
		subMeshes.clear();
	}
}

void Model::Render(Camera* camera_) {
	for (auto m : subMeshes) {
		m->Render(camera_, modelInstances);
	}
}

void Model::AddMesh(Mesh* mesh_) {
	subMeshes.push_back(mesh_);
}

glm::mat4 Model::GetTransform(int modelIndex_) const {
	return modelInstances[modelIndex_];
}

int Model::CreateInstance(glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scale_) {
	totalInstances++; // Increase the total number of instances
	for (int i = 0; i < modelInstances.size(); i++) { // Attempt to find an empty slot inside of the model instances vector
		if (modelInstances[i] == glm::mat4(0.0f)) {
			modelInstances[i] = GetTransform(position_, rotation_, angle_, scale_);
			return i; // Return that inner index value
		}
	}
	// If no empty slots exists, place the instance at the end of the vector
	modelInstances.push_back(GetTransform(position_, rotation_, angle_, scale_));
	return modelInstances.size() - 1;
}

void Model::RemoveInstance(int modelIndex_) {
	totalInstances--; // Decrease the total number of instances
	modelInstances[modelIndex_] = glm::mat4(0.0f);
	//if (totalInstances <= 0) { // If no game objects associated with this model exist, remove the model from memory
	//	SceneGraph::GetInstance()->RemoveModel(shaderProgram, this);
	//} I have no idea if removing the model actually works or not so it's commented out for now. I don't want memory leaks :(
}

void Model::UpdateInstance(int modelIndex_, glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scale_) {
	modelInstances[modelIndex_] = GetTransform(position_, rotation_, angle_, scale_);
}

BoundingBox Model::GetBoundingBox() {
	return box;
}

GLuint Model::GetShaderProgram() const {
	return shaderProgram;
}

glm::mat4 Model::GetTransform(glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scale_) const {
	glm::mat4 model;
	model = glm::translate(model, position_);
	model = glm::rotate(model, angle_, rotation_);
	model = glm::scale(model, scale_);
	return model;
}

void Model::LoadModel(const std::string &objFilePath_, const std::string &matFilePath_) {
	LoadModelOBJ* object = new LoadModelOBJ();
	object->LoadModel(objFilePath_, matFilePath_);
	for (auto o : object->GetSubMeshes()) {
		subMeshes.push_back(new Mesh(o, shaderProgram));
	}
	box = object->GetBoundingBox();
	delete object, object = nullptr;
}