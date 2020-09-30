#ifndef TEXTURE_HANDLER_H
#define TEXTURE_HANDLER_H

#include <memory>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include "../Core/DebugLogger.h"

struct Texture {
	float width = 0.0f;
	float height = 0.0f;
	GLuint textureID = 0;
	std::string textureName = "";
};

class TextureHandler {
private:
	static std::vector<Texture*> textures;

	static std::unique_ptr<TextureHandler> instance;
	friend std::default_delete<TextureHandler>;
public:
	TextureHandler(const TextureHandler&) = delete;
	TextureHandler(TextureHandler&&) = delete;
	TextureHandler& operator=(const TextureHandler&) = delete;
	TextureHandler& operator=(TextureHandler&&) = delete;

	void OnDestroy();

	void CreateTexture(const std::string& textureName_, const std::string& filePath_);
	static const GLuint GetTexture(const std::string& textureName_);
	static const Texture* GetTextureData(const std::string& textureName_);

	static TextureHandler* GetInstance();
private:
	TextureHandler();
	~TextureHandler();
};

#endif