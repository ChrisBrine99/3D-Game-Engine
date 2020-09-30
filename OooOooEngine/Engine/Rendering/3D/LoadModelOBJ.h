#ifndef LOAD_MODEL_OBJ_H
#define LOAD_MODEL_OBJ_H

#include <sstream>
#include <algorithm>
#include "../../Core/DebugLogger.h"
#include "../../FX/MaterialLoader.h"
#include "../../Math/BoundingBox.h"
#include "Mesh.h"

class LoadModelOBJ {
private:
	Material curMaterial;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texCoords;

	std::vector<int> indices;
	std::vector<int> normalIndices;
	std::vector<int> texIndices;

	std::vector<Vertex> meshVertices;
	std::vector<SubMesh> subMeshes;

	BoundingBox box;
public:
	LoadModelOBJ();
	~LoadModelOBJ();

	void OnDestroy();

	void LoadModel(const std::string &objFilePath_);
	void LoadModel(const std::string &objFilePath_, const std::string &matFilePath_);

	std::vector<Vertex> GetVerts();
	std::vector<int> GetIndices();
	std::vector<SubMesh> GetSubMeshes();

	BoundingBox GetBoundingBox();
private:
	void PostProcessing();
	void LoadMaterial(const std::string &filename_);
	void LoadMaterialLibrary(const std::string &matFilePath_);
};

#endif