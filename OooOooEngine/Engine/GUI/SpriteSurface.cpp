#include "SpriteSurface.h"
#include "../Graphics/ShaderHandler.h"
#include <glm/gtc/matrix_transform.hpp>

SpriteSurface::SpriteSurface(const std::string& _texture, glm::vec2 _scale, float _angle, glm::vec4 _tint) :
	VAO(0), VBO(0), modelLocation(0), projLocation(0), texLocation(0) {
	textureID = TextureHandler::GetInstance()->GetTexture(_texture);
	if (textureID == 0) {
		DebugLogger::Error("Couldn't load the texture!", "SpriteSurface.cpp", __LINE__);
	}

	const Texture* textureData = TextureHandler::GetInstance()->GetTextureData(_texture);
	size.x = textureData->height;
	size.y = textureData->width;

	scale = _scale;
	angle = _angle;
	tint = _tint;

	// Creating the VAO and VBO
	struct VertexData {
		glm::vec2 position;
		glm::vec2 texCoords;
	};

	VertexData data[] = {
		{ glm::vec2(-0.5f, 0.5f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 1.0f) },
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, texCoords));

	// Load in the GUI Shader
	GLuint shader = ShaderHandler::GetInstance()->GetShader("SpriteShader");
	modelLocation = glGetUniformLocation(shader, "model");
	projLocation = glGetUniformLocation(shader, "proj");
	tintLocation = glGetUniformLocation(shader, "tintColour");
	texLocation = glGetUniformLocation(shader, "inputTexture");
}

SpriteSurface::~SpriteSurface() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void SpriteSurface::Draw(Camera* camera, const glm::vec2 _position) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Calculate the position of the sprite on screen
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(_position, 0.0f));
	model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x * size.x, scale.y * size.y, 1.0f));

	// Set the sprite's uniforms
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, glm::value_ptr(camera->GetOrthographic()));
	glUniform4f(tintLocation, tint.x, tint.y, tint.z, tint.w);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}