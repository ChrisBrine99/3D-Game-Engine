#include "MaterialHandler.h"

std::vector<Material> MaterialHandler::materials = std::vector<Material>();
std::unique_ptr<MaterialHandler> MaterialHandler::instance = nullptr;

MaterialHandler::MaterialHandler() {
	materials.reserve(10);
}

MaterialHandler::~MaterialHandler() {
	OnDestroy();
}

void MaterialHandler::OnDestroy() {
	if (materials.size() > 0) {
		materials.clear();
	}
}

void MaterialHandler::AddMaterial(Material material_) {
	materials.push_back(material_);
}

Material MaterialHandler::GetMaterial(const std::string materialName_) const {
	for (auto m : materials) {
		if (m.name == materialName_) {
			return m;
		}
	}
	return Material();
}

MaterialHandler* MaterialHandler::GetInstance() {
	if (instance.get() == nullptr) {
		instance.reset(new MaterialHandler);
	}
	return instance.get();
}