#ifndef MATERIAL_HANDLER_H
#define MATERIAL_HANDLER_H

#include <memory>
#include <glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Material {
	std::string name;
	GLuint diffuseMap;

	float shine;		// Ns in .obj
	float alpha;		// d  in .obj

	glm::vec3 ambient;	// Ka in .obj
	glm::vec3 diffuse;	// Kd in .obj
	glm::vec3 specular;	// Ks in .obj

	inline Material() {
		name = "";
		diffuseMap = 0;

		shine = 0.0f;
		alpha = 0.0f;

		ambient = glm::vec3(1.0f);
		diffuse = glm::vec3(1.0f);
		specular = glm::vec3();
	}
};

class MaterialHandler {
private:
	static std::vector<Material> materials;

	static std::unique_ptr<MaterialHandler> instance;
	friend std::default_delete<MaterialHandler>;
public:
	MaterialHandler(const MaterialHandler&) = delete;
	MaterialHandler(MaterialHandler&&) = delete;
	MaterialHandler& operator=(const MaterialHandler&) = delete;
	MaterialHandler& operator=(MaterialHandler&&) = delete;

	MaterialHandler();
	~MaterialHandler();

	void OnDestroy();

	void AddMaterial(Material material_);
	Material GetMaterial(const std::string materialName_) const;

	static MaterialHandler* GetInstance();
};

#endif