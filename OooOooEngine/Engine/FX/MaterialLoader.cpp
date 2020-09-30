#include "MaterialLoader.h"

MaterialLoader::~MaterialLoader() {
}

void MaterialLoader::LoadMaterial(std::string filePath_) {
	std::ifstream in(filePath_.c_str(), std::ios::in);
	if (!in) { // The material file couldn't be opened; exit to prevent loading
		DebugLogger::Error("Material file could not be read. (.mtl)", "MaterialLoader.cpp", __LINE__);
		return;
	}
	Material m = Material();
	std::string mName = "";
	std::string line;
	while (getline(in, line)) { // Keep looping until the last line of the file
		if (line.substr(0, 7) == "newmtl ") { // Loading in a new material
			if (m.diffuseMap != 0) {
				MaterialHandler::GetInstance()->AddMaterial(m);
				m = Material();
			}
			mName = line.substr(7);
			m.diffuseMap = LoadTexture(mName);
			m.name = mName;
		} else if (line.find('\t') != std::string::npos) {
			line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
			if (line.substr(0, 3) == "Ns ") { // Loading in shine data
				std::istringstream s(line.substr(3));
				s >> m.shine;
			} else if (line.substr(0, 2) == "d ") { // Loading in alpha data
				std::istringstream s(line.substr(2));
				s >> m.alpha;
			} else if (line.substr(0, 3) == "Ka ") { // Loading in ambient data
				std::istringstream a(line.substr(3));
				a >> m.ambient.x >> m.ambient.y >> m.ambient.z;
			} else if (line.substr(0, 3) == "Kd ") { // Loading in diffuse data
				std::istringstream d(line.substr(3));
				d >> m.diffuse.x >> m.diffuse.y >> m.diffuse.z;
			} else if (line.substr(0, 3) == "Ks ") { // Loading in specular data
				std::istringstream s(line.substr(3));
				s >> m.specular.x >> m.specular.y >> m.specular.z;
			}
		}
	}
	in.close();
	if (m.diffuseMap != 0) { // Load the final material into the vector
		MaterialHandler::GetInstance()->AddMaterial(m);
	}
}

GLuint MaterialLoader::LoadTexture(std::string filename_) {
	GLuint currentTexture = TextureHandler::GetInstance()->GetTexture(filename_);
	if (currentTexture == 0) { // Texture doesn't exist yet; create it
		TextureHandler::GetInstance()->CreateTexture(filename_, "Resources/Textures/" + filename_ + ".jpg");
		currentTexture = TextureHandler::GetInstance()->GetTexture(filename_);
	}
	return currentTexture;
}