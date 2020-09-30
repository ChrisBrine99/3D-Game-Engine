#include "Mesh.h"

Mesh::Mesh(SubMesh subMesh_, GLuint shaderProgram_) {
	subMesh = subMesh_;
	shaderProgram = shaderProgram_;
	VAO = 0;
	VBO = 0;

	modelLocation = 0;
	viewLocation = 0;
	projLocation = 0;

	camPosLocation = 0;

	matTexLocation = 0;
	matShineLocation = 0;
	matAlphaLocation = 0;
	matAmbLocation = 0;
	matDiffLocation = 0;
	matSpecLocation = 0;

	GenerateBuffers();
}

Mesh::~Mesh() {
	subMesh.meshIndices.clear();
	subMesh.vertexList.clear();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::Render(Camera* camera_, std::vector<glm::mat4> instances_) {
	glUniform1i(matTexLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, subMesh.material.diffuseMap);
	// Set Uniforms
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, value_ptr(camera_->GetView()));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, value_ptr(camera_->GetPerspective()));

	glUniform3fv(camPosLocation, 1, value_ptr(camera_->GetPosition()));

	/*for (int i = 0; i < camera_->GetLights().size(); i++) {
		glUniform3fv(lightPosLocation[i], 1, value_ptr(camera_->GetLights()[i]->GetPosition()));
		glUniform3fv(lightColLocation[i], 1, value_ptr(camera_->GetLights()[i]->GetColor()));
		glUniform1f(lightAmbLocation[i], camera_->GetLights()[i]->GetAmbient());
		glUniform3fv(lightAttenLocation[i], 1, value_ptr(camera_->GetLights()[i]->GetAttenuation()));
	}*/
	glUniform3fv(lightPosLocation, 1, value_ptr(camera_->GetLights()[0]->GetPosition()));
	glUniform3fv(lightColLocation, 1, value_ptr(camera_->GetLights()[0]->GetColor()));
	glUniform1f(lightAmbLocation, camera_->GetLights()[0]->GetAmbient());

	glUniform1f(matShineLocation, subMesh.material.shine);
	glUniform1f(matAlphaLocation, subMesh.material.alpha);
	glUniform3fv(matDiffLocation, 1, value_ptr(subMesh.material.diffuse));
	glUniform3fv(matAmbLocation, 1, value_ptr(subMesh.material.ambient));
	glUniform3fv(matSpecLocation, 1, value_ptr(subMesh.material.specular));
	// Draw the mesh
	glBindVertexArray(VAO);
	for (auto i : instances_) {
		if (i == glm::mat4(0.0f) || !CollisionDetection::OBBInFrustum(i)) { // The model matrix is empty; don't bother rendering
			continue;
		}
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(i));
		glDrawArrays(GL_TRIANGLES, 0, subMesh.vertexList.size());
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::GenerateBuffers() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, subMesh.vertexList.size() * sizeof(Vertex), &subMesh.vertexList[0], GL_STATIC_DRAW);

	// Position Data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Normal Data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Texture Coordinate Data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureCoord));
	// Color Data
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Model Screen Data
	modelLocation = glGetUniformLocation(shaderProgram, "model");
	viewLocation = glGetUniformLocation(shaderProgram, "view");
	projLocation = glGetUniformLocation(shaderProgram, "projection");

	// Camera Location Data
	camPosLocation = glGetUniformLocation(shaderProgram, "cameraPos");

	// Light Data
	lightPosLocation = glGetUniformLocation(shaderProgram, "light.position");
	lightColLocation = glGetUniformLocation(shaderProgram, "light.color");
	lightAmbLocation = glGetUniformLocation(shaderProgram, "light.ambient");
	/*for (int i = 0; i < MAX_LIGHTS; i++) {
		std::string curIndex = std::to_string(i);
		std::string temp = "light.position[" + curIndex + "]"; // The position data of the current index
		lightPosLocation[i] = glGetUniformLocation(shaderProgram, temp.c_str());
		temp = "light.color[" + curIndex + "]"; // The color data of the current index
		lightColLocation[i] = glGetUniformLocation(shaderProgram, temp.c_str());
		temp = "light.ambient[" + curIndex + "]"; // The ambient data of the current index
		lightAmbLocation[i] = glGetUniformLocation(shaderProgram, temp.c_str());
		temp = "light.attenuation[" + curIndex + "]"; // The attenuation data of the current index
		lightAttenLocation[i] = glGetUniformLocation(shaderProgram, temp.c_str());
	}*/

	// Material Data
	matTexLocation = glGetUniformLocation(shaderProgram, "material.diffuseMap");
	matShineLocation = glGetUniformLocation(shaderProgram, "material.shine");
	matAlphaLocation = glGetUniformLocation(shaderProgram, "material.alpha");
	matAmbLocation = glGetUniformLocation(shaderProgram, "material.ambient");
	matDiffLocation = glGetUniformLocation(shaderProgram, "material.diffuse");
	matSpecLocation = glGetUniformLocation(shaderProgram, "material.specular");
}
