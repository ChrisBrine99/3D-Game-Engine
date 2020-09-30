#ifndef MATERIAL_LOADER_H
#define MATERIAL_LOADER_H

#include "../Graphics/MaterialHandler.h"
#include "../Graphics/TextureHandler.h"
#include <algorithm>
#include <sstream>

class MaterialLoader {
public:
	MaterialLoader(const MaterialLoader&) = delete;
	MaterialLoader(MaterialHandler&&) = delete;
	MaterialLoader& operator=(const MaterialLoader&) = delete;
	MaterialLoader& operator=(MaterialLoader&&) = delete;

	MaterialLoader() = delete;
	~MaterialLoader();

	static void LoadMaterial(std::string filepath_);
private:
	static GLuint LoadTexture(std::string filename_);
};

#endif