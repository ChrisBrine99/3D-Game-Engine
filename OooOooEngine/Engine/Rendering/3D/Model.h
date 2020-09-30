#ifndef MODEL_H
#define MODEL_H

#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include "LoadModelOBJ.h"

class Model {
private:
	int totalInstances;

	LoadModelOBJ* object;
	std::vector<glm::mat4> modelInstances;
	std::vector<Mesh*> subMeshes;
	GLuint shaderProgram;
	BoundingBox box;
public:
	Model(const std::string &objFilePath_, const std::string &matFilePath_, GLuint shaderProgram_);
	~Model();

	void Render(Camera* camera_);
	void AddMesh(Mesh* mesh_);

	glm::mat4 GetTransform(int modelIndex_) const;
	int CreateInstance(glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scale_);
	void RemoveInstance(int modelIndex_);
	void UpdateInstance(int modelIndex_, glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scale_);

	BoundingBox GetBoundingBox();

	GLuint GetShaderProgram() const;
private:
	glm::mat4 GetTransform(glm::vec3 position_, glm::vec3 rotation_, float angle_, glm::vec3 scale_) const;
	void LoadModel(const std::string &objFilePath_, const std::string &matFilePath_);
};

#endif