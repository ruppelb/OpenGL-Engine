#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Material.h"

#include "CameraController.h"
#include "LightController.h"
#include "scene_objects/SceneObject.h"
#include "scene_objects/LightObject.h"
#include "scene_objects/CameraObject.h"
#include "ObjectLoader.h"

//maintains list of gameobjects. 
class SceneManager
{
public:

	static SceneManager* getInstance();

	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	void init(int width, int height);

	std::shared_ptr<Renderer> loadInitialScene();

	std::shared_ptr<SceneObject> addSceneObjectFromFile(std::string name, std::string filepath, Meshtype meshType, Shader shader); //for adding generic GameObjects. Has to specify path to Obj, Meshtype, Shader type and Name. Loads obj with meshtype. Creates Sceneobject with mesh, shader and name. TODO: add enumeration type for shader?
	std::shared_ptr<SOCamera> addCameraObject(glm::vec3 position, CameraType type); //for adding Cameras. uses preset mesh loaded initially.
	std::shared_ptr<SOLight> addLightObject(glm::vec3 position, LightType type); //for adding light sources. uses preset mesh loaded initially. Position only used in case of point light

	void updateObjects(float deltaTime); //calls onUpdate on all Objects
	void renderObjects(); //calls onRender on all Objects
	void renderImGui();

private:
	static SceneManager* inst_;   // The one, single instance
	SceneManager() {};
	~SceneManager();

	void loadAssets();
	glm::mat4 createProj(int in_width, int in_height);

	int m_width;
	int m_height;

	ObjectLoader* m_objL = nullptr;
	std::shared_ptr<Renderer> m_renderer;

	CameraController* cameraController;
	LightController* lightController;

	std::vector<std::shared_ptr<Texture> > ui_textures;
	std::vector<std::shared_ptr<SceneObject> > sceneObjects;
	std::vector<std::shared_ptr<Mesh>> cubeMesh;
	std::vector<std::shared_ptr<Mesh>> cameraMesh;
	std::vector<std::shared_ptr<Mesh>> pointLightMesh;
	std::vector<std::shared_ptr<Mesh>> directionalLightMesh;
	std::vector<std::shared_ptr<Mesh>> desertMesh;

	std::shared_ptr<Shader> m_materialShader;
	std::shared_ptr<Shader> m_normalShader;

};

