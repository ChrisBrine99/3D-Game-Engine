#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <memory>
#include <vector>
#include <map>
#include "3D/GameObject.h"
#include "../GUI/GUIObject.h"
#include "../Math/CollisionHandler.h"

class SceneGraph {
private:
	static int totalObjectsAdded;

	static std::map<GLuint, std::vector<Model*>> sceneModels;
	static std::map<std::string, GameObject*> sceneObjects;
	static std::map<std::string, GUIObject*> sceneGUIObjects;

	static std::unique_ptr<SceneGraph> instance;
	friend std::default_delete<SceneGraph>;
public:
	SceneGraph(const SceneGraph&) = delete;
	SceneGraph(SceneGraph&&) = delete;
	SceneGraph& operator=(const SceneGraph&) = delete;
	SceneGraph& operator=(SceneGraph&&) = delete;

	SceneGraph();
	~SceneGraph();
	void OnDestroy();

	void Update(const float deltaTime_);
	void Render(Camera* camera_);
	void Draw(Camera* _camera);

	static SceneGraph* GetInstance();

	void AddModel(Model* model_);
	//void RemoveModel(GLuint program_, Model* model_);
	void AddGameObject(GameObject* gameObject_, std::string name_ = "");
	void RemoveGameObject(std::string name_);
	GameObject* GetGameObject(std::string name_);

	void AddGUIObject(GUIObject* _guiObject, std::string _name = "");
	void RemoveGUIObject(std::string _name);
	GUIObject* GetGUIObject(std::string _name);
};

#endif