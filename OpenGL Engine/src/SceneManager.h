#pragma once
#include "scene_objects/SceneObject.h"
#include "scene_objects/LightObject.h"

//maintains list of gameobjects. 
class SceneManager
{
public:
	void addSceneObject(std::string name, std::string filepath, Meshtype meshType, Shader shader); //for adding generic GameObjects. Has to specify path to Obj, Meshtype, Shader type and Name. Loads obj with meshtype. Creates Sceneobject with mesh, shader and name. TODO: add enumeration type for shader?
	void addCameraObject(glm::vec3 position); //for adding Cameras. uses preset mesh loaded initially.
	void addLightObject(int lightId, glm::vec3 position); //for adding light sources. uses preset mesh loaded initially. Position only used in case of point light

	void updateObjects(); //calls onUpdate on all Objects
	void renderObjects(); //calls onRender on all Objects
};

