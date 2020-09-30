#ifndef MESH_H
#define MESH_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glew.h>
#include <vector>
#include <string>
#include <glm/gtx/string_cast.hpp>
#include "../../Math/CollisionDetection.h"
#include "../../Camera/Camera.h"
#include "../../Graphics/MaterialHandler.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 textureCoord;
};

struct SubMesh {
	std::vector<Vertex> vertexList;
	std::vector<int> meshIndices;
	Material material;
};

class Mesh {
private:
	SubMesh subMesh;
	GLuint VAO, VBO;
	GLuint shaderProgram;

	GLuint modelLocation;
	GLuint viewLocation;
	GLuint projLocation;

	GLuint camPosLocation;

	GLuint lightPosLocation;
	GLuint lightColLocation;
	GLuint lightAmbLocation;

	GLuint matTexLocation;
	GLuint matShineLocation;
	GLuint matAlphaLocation;
	GLuint matAmbLocation;
	GLuint matDiffLocation;
	GLuint matSpecLocation;
public:
	Mesh(SubMesh subMesh_, GLuint shaderProgram_);
	~Mesh();

	void Render(Camera* camera_, std::vector<glm::mat4> instances_);
private:
	void GenerateBuffers();
};

#endif