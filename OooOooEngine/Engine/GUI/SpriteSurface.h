#ifndef SPRITE_SURFACE_H
#define SPRITE_SURFACE_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glew.h>
#include <string>
#include "../Graphics/TextureHandler.h"
#include "../Camera/Camera.h"

class SpriteSurface {
private:
	GLuint textureID;
	GLuint VAO, VBO;
	
	GLuint modelLocation;
	GLuint projLocation;
	GLuint tintLocation;
	GLuint texLocation;

	glm::vec4 tint;
	glm::ivec2 size;
	glm::vec2 scale;
	float angle;
public:
	SpriteSurface(const std::string& _texture, glm::vec2 _scale, float _angle, glm::vec4 _tint);
	~SpriteSurface();

	void Draw(Camera* camera, const glm::vec2 _position);

	// Getter and Setters
	glm::vec2 GetScale() const { return scale; }
	glm::vec2 GetSize() const { return size; }
	glm::vec2 GetScaledSize() const { return scale * glm::vec2(size); }
private:
	void GenerateBuffers();
};

#endif