#include "LoadModelOBJ.h"

LoadModelOBJ::LoadModelOBJ() {
	curMaterial = Material();

	vertices = std::vector<glm::vec3>();
	vertices.reserve(200);
	normals = std::vector<glm::vec3>();
	normals.reserve(200);
	texCoords = std::vector<glm::vec2>();
	texCoords.reserve(200);
	
	indices = std::vector<int>();
	indices.reserve(200);
	normalIndices = std::vector<int>();
	normalIndices.reserve(200);
	texIndices = std::vector<int>();
	texIndices.reserve(200);
	
	meshVertices = std::vector<Vertex>();
	meshVertices.reserve(200);
	subMeshes = std::vector<SubMesh>();
	subMeshes.reserve(10);
}

LoadModelOBJ::~LoadModelOBJ() {
	OnDestroy();
}

void LoadModelOBJ::OnDestroy() {
	vertices.clear();
	normals.clear();
	texCoords.clear();

	indices.clear();
	normalIndices.clear();
	texIndices.clear();

	meshVertices.clear();
	subMeshes.clear();
}

void LoadModelOBJ::PostProcessing() {
	Vertex vertex;
	for (unsigned int i = 0; i < indices.size(); i++) {
		vertex.position = vertices[indices[i]];
		vertex.textureCoord = texCoords[texIndices[i]];
		vertex.normal = normals[normalIndices[i]];
		meshVertices.push_back(vertex);
	}
	SubMesh subMesh;
	subMesh.vertexList = meshVertices;
	subMesh.meshIndices = indices;
	subMesh.material = curMaterial;
	subMeshes.push_back(subMesh);
	// Resetting the necessary variables
	curMaterial = Material();
	indices.clear();
	normalIndices.clear();
	texIndices.clear();
	meshVertices.clear();
}

void LoadModelOBJ::LoadMaterial(const std::string &filename_) {
	curMaterial = MaterialHandler::GetInstance()->GetMaterial(filename_);
}

void LoadModelOBJ::LoadMaterialLibrary(const std::string &matFilePath_) {
	MaterialLoader::LoadMaterial(matFilePath_);
}

void LoadModelOBJ::LoadModel(const std::string &objFilePath_) {
	std::ifstream in(objFilePath_.c_str(), std::ios::in);
	if (!in) {
		DebugLogger::Error("Object file could not be read. (.obj)", "LoadModelOBJ.cpp", __LINE__);
		return;
	}
	std::string line;
	while (getline(in, line)) { // Keep looping until the last line of the file
		if (line.substr(0, 2) == "v ") { // Loading in vertex data
			std::istringstream v(line.substr(2));
			glm::vec3 vertPosition;
			v >> vertPosition.x >> vertPosition.y >> vertPosition.z;
			vertices.push_back(vertPosition);
		} else if (line.substr(0, 3) == "vn ") { // Loading in normal data
			std::istringstream n(line.substr(3));
			glm::vec3 normPosition;
			n >> normPosition.x >> normPosition.y >> normPosition.z;
			normals.push_back(normPosition);
		} else if (line.substr(0, 3) == "vt ") { // Loading in texture coordinate data
			std::istringstream c(line.substr(3));
			glm::vec2 texCoordPosition;
			c >> texCoordPosition.x >> texCoordPosition.y;
			texCoords.push_back(texCoordPosition);
		} else if (line.substr(0, 2) == "f "){ // Loading in face data
			std::istringstream f(line.substr(2));
			std::string dataGroups[3]; // First, split the line of data into three strings containing "n/n/n"
			f >> dataGroups[0] >> dataGroups[1] >> dataGroups[2];
			// Next, split each of these strings into a individual chunks of data; placing that data into its respective vector
			std::string token = "";
			for (int i = 0; i < 3; i++) {
				std::stringstream data(dataGroups[i]);
				std::vector<std::string> tokens;
				while (getline(data, token, '/')) { // Split the data by the '/' character
					tokens.push_back(token);
				}
				// Place the correct element into the correct vector "indices/texIndices/normalIndices" AKA element "0/1/2"
				indices.push_back(stoi(tokens[0]) - 1);
				texIndices.push_back(stoi(tokens[1]) - 1);
				normalIndices.push_back(stoi(tokens[2]) - 1);
			}
		} else if (line.substr(0, 7) == "usemtl ") { // Gathering all the data; moving onto the next material as well
			if (indices.size() > 0) {
				PostProcessing();
			}
			LoadMaterial(line.substr(7));
		}
	}
	// Ensures the last chunk of data is gathered and retained in memory
	PostProcessing();
}

void LoadModelOBJ::LoadModel(const std::string &objFilePath_, const std::string &matFilePath_) {
	LoadMaterialLibrary(matFilePath_);
	LoadModel(objFilePath_);
}

std::vector<Vertex> LoadModelOBJ::GetVerts() {
	return meshVertices;
}

std::vector<int> LoadModelOBJ::GetIndices() {
	return indices;
}

std::vector<SubMesh> LoadModelOBJ::GetSubMeshes() {
	return subMeshes;
}

BoundingBox LoadModelOBJ::GetBoundingBox() {
	if (vertices.size() > 0) {
		box.minVert = vertices[0];
		box.maxVert = vertices[0];
		for (int i = 1; i < vertices.size(); i++) {
			box.minVert.x = box.minVert.x < vertices[i].x ? box.minVert.x : vertices[i].x;
			box.minVert.y = box.minVert.y < vertices[i].y ? box.minVert.y : vertices[i].y;
			box.minVert.z = box.minVert.z < vertices[i].z ? box.minVert.z : vertices[i].z;

			box.maxVert.x = box.maxVert.x > vertices[i].x ? box.maxVert.x : vertices[i].x;
			box.maxVert.y = box.maxVert.y > vertices[i].y ? box.maxVert.y : vertices[i].y;
			box.maxVert.z = box.maxVert.z > vertices[i].z ? box.maxVert.z : vertices[i].z;
		}
		return box;
	}
	return BoundingBox();
}