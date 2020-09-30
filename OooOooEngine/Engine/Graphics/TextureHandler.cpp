#include "TextureHandler.h"

std::unique_ptr<TextureHandler> TextureHandler::instance = nullptr;
std::vector<Texture*> TextureHandler::textures = std::vector<Texture*>();

TextureHandler::TextureHandler() {
	textures.reserve(10);
}

TextureHandler::~TextureHandler() {
	OnDestroy();
}

void TextureHandler::OnDestroy() {
	if (textures.size() > 0) {
		for (auto t : textures) {
			glDeleteTextures(sizeof(GLuint), &t->textureID);
			delete t, t = nullptr;
		}
		textures.clear();
	}
}

void TextureHandler::CreateTexture(const std::string &textureName_, const std::string &filePath_) {
	Texture* texture = new Texture();
	SDL_Surface* surface = IMG_Load(filePath_.c_str());
	if (surface == nullptr) {
		DebugLogger::Error("Surface for texture " + textureName_ + " couldn't be created.", "TextureHandler.cpp", __LINE__);
		delete texture, texture = nullptr;
		return;
	}
	glGenTextures(1, &texture->textureID);
	glBindTexture(GL_TEXTURE_2D, texture->textureID);
	int mode = surface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	texture->width = surface->w;
	texture->height = surface->h;
	texture->textureName = textureName_;
	textures.push_back(texture);

	SDL_FreeSurface(surface);
	surface = nullptr;
}

const GLuint TextureHandler::GetTexture(const std::string &textureName_) {
	for (auto t : textures) {
		if (t->textureName == textureName_) {
			return t->textureID;
		}
	}
	return 0;
} 

const Texture* TextureHandler::GetTextureData(const std::string &textureName_) {
	for (auto t : textures) {
		if (t->textureName == textureName_) {
			return t;
		}
	}
	return nullptr;
}

TextureHandler* TextureHandler::GetInstance() {
	if (instance.get() == nullptr) {
		instance.reset(new TextureHandler);
	}
	return instance.get();
}