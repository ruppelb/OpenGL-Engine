#include "SceneManager.h"

SceneManager* SceneManager::inst_ = NULL;

SceneManager* SceneManager::getInstance()
{
	if (inst_ == NULL) {
		inst_ = new SceneManager();
	}
	return (inst_);
}

void SceneManager::init(int width, int height)
{	
	m_width = width;
	m_height = height;
}

std::shared_ptr<Renderer> SceneManager::loadInitialScene()
{
	loadAssets();


	//setup Lighting
	lightController = LightController::getInstance();

	//create CameraController
	cameraController = CameraController::getInstance();

	//create renderer
	std::shared_ptr<Renderer> m_renderer;

	//create initial objects
	 
	//create scene grid
	gridObject = std::make_shared<GridObject>(m_objL->loadPlaneV(), m_gridShader, m_renderer);
	
	//cameras
	std::shared_ptr<SOCamera> cameraZero = addCameraObject(glm::vec3(10.0, 10.0, 5.0), Perspective);
	cameraZero->setFront(glm::normalize(glm::vec3(-1.0, -1.0, -1.0)));
	std::shared_ptr<SOCamera> cameraOne = addCameraObject(glm::vec3(0.0, 0.0, 0.0), Ortho);
	
	//light
	std::shared_ptr<SOLight> lightZero = addLightObject(glm::vec3(1.0, -1.0, -1.0), Directional);
	std::shared_ptr<SOLight> lightOne = addLightObject(glm::vec3(10, 0.0, 0.0), Point);


	//object
	std::shared_ptr<SceneObject> test = std::make_shared<SceneObject>("TestMesh", desertMesh, m_materialShader, m_renderer);
	//test->setModelMatrix(glm::rotate(glm::translate(glm::mat4(1.0), ), glm::radians(45.0f), glm::vec3(0.0f,	1.0f, 0.0f)));
	test->setTranslation(glm::vec3(0, 0, -10));
	test->setRotationY(45.0f);
	test->setScale(glm::vec3(0.1));
	test->setHidden(false);
	sceneObjects.push_back(test);

	/*
	std::shared_ptr<Mesh> t = m_objL->loadCubeVTN(Lines);
	std::shared_ptr<SOLine> test2 = std::make_shared<SOLine>("TestLine", t, m_materialShader, m_renderer);
	test2->setHidden(false);
	sceneObjects.push_back(test2);*/

	return m_renderer;
}

std::shared_ptr<SceneObject> SceneManager::addSceneObjectFromFile(std::string name, std::string filepath, MeshType meshType, Shader shader)
{
	return nullptr;
}

std::shared_ptr<SOCamera> SceneManager::addCameraObject(glm::vec3 position, CameraType type)
{
	std::shared_ptr<SOCamera> cameraObject;
	std::string name = "Camera";
	int id = -1;
	switch (type) {
	case Ortho:
		id = cameraController->addOrthoCamera(position, glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0, 1.0, 0.0), -8, 8, -6, 6,0.1,500);
		if (id > -1) {
			name.append(std::to_string(id - 1));
			cameraObject = std::make_shared<SOCamera>(id, name, cameraMesh, m_materialShader, m_renderer);
			cameraObject->setHidden(false);
			sceneObjects.push_back(cameraObject);
			return cameraObject;
		}
		break;
	case Perspective:
		//addPerpectiveCamera(position, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), glm::radians(30.0), 4/3.0, 0.1, 500);
		id = cameraController->addCamera(position, glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0), createProj(m_width, m_height),0.1,500);
		if (id > -1) {
			name.append(std::to_string(id - 1));
			cameraObject = std::make_shared<SOCamera>(id, name, cameraMesh, m_materialShader, m_renderer);
			cameraObject->setHidden(false);
			sceneObjects.push_back(cameraObject);
			return cameraObject;
		}
		break;
	}
	return nullptr;
}

std::shared_ptr<SOLight> SceneManager::addLightObject(glm::vec3 position, LightType type)
{
	std::shared_ptr<SOLight> lightObject;
	std::string name = "Light";
	int id = -1;
	switch (type) {
	case Point:
		id = lightController->addLightSource(position, glm::vec4(1.0, 1.0, 1.0, 1.0));
		if (id > -1) {
			name.append(std::to_string(id - 1));
			lightObject = std::make_shared<SOLight>(id, name, pointLightMesh, m_materialShader, m_renderer);
			lightObject->setHidden(false);
			sceneObjects.push_back(lightObject);
			return lightObject;
		}
		break;
	case Directional:
		id = lightController->addDirectionalLightSource(position, glm::vec4(1.0, 1.0, 1.0, 1.0));
		if (id > -1) {
			name.append(std::to_string(id - 1));
			lightObject = std::make_shared<SOLight>(id, name, directionalLightMesh, m_materialShader, m_renderer);
			lightObject->setHidden(false);
			sceneObjects.push_back(lightObject);
		}
		return lightObject;
		break;
	}

	return nullptr;
}

void SceneManager::updateObjects(float deltaTime)
{
	gridObject->onUpdate(deltaTime);

	for (std::shared_ptr<SceneObject> object : sceneObjects) {
		object->onUpdate(deltaTime);
	}
}

void SceneManager::renderObjects()
{

	
	
	for (std::shared_ptr<SceneObject> object : sceneObjects) {
		object->onRender();
	}

	//render scene grid last, so that it does not obstruct other objects
	gridObject->onRender();
}

void SceneManager::renderImGui()
{
	for (std::shared_ptr<SceneObject> object : sceneObjects) {
		//if (object == lightOne) {
		object->onImGuiRender();
		//}
	}
}

SceneManager::~SceneManager()
{
	sceneObjects.clear();
	ui_textures.clear();
}

void SceneManager::loadAssets()
{
	//load objects
	m_objL = ObjectLoader::getInstance();
	//std::vector<Mesh> meshesObj1 = objL->loadObjectFromFile("res/objects/monkey.obj", Meshtype::PN);
	//Mesh cubeMesh = objL->loadCubeVTN();

	cubeMesh = m_objL->loadObjectFromFile("res/objects/cube.obj", MeshType::PNC);
	desertMesh = m_objL->loadObjectFromFile("res/objects/desert.obj", MeshType::PNC);
	cameraMesh = m_objL->loadObjectFromFile("res/objects/camera.obj", MeshType::PNC);
	pointLightMesh = m_objL->loadObjectFromFile("res/objects/pointLight.obj", MeshType::PNC);
	directionalLightMesh = m_objL->loadObjectFromFile("res/objects/directionalLight.obj", MeshType::PNC);

	//load shaders
	m_materialShader = std::make_shared<Shader>("res/shaders/lightingMaterial.shader");
	m_normalShader = std::make_shared<Shader>("res/shaders/normal.shader");
	m_gridShader = std::make_shared<Shader>("res/shaders/grid.shader");

	//load textures
	//std::shared_ptr<Texture> containerDiffuseTexture = std::make_shared<Texture>("res/textures/container_d.png");
	//std::shared_ptr<Texture> containerSpecularTexture = std::make_shared<Texture>("res/textures/container_s.png");

	//create materials
	//Material gold (glm::vec4(0.24725,0.1995,0.0745,1.0),glm::vec4(0.75164,0.60648,0.22648,1.0),glm::vec4(0.628281,0.555802,0.366065,1.0),0.1);
	//Material box(containerDiffuseTexture, containerSpecularTexture, 10.0,shader);
}

glm::mat4 SceneManager::createProj(int in_width, int in_height)
{
	float near = 0.1f;
	float far = 500.0f;
	int max_d = cv::max(in_width, in_height);
	float fx = max_d;
	float fy = max_d;
	float cx = (float)in_width / 2.0f;
	float cy = (float)in_height / 2.0f;

	float A = -(far + near) / (far - near);
	float B = -(2 * far * near) / (far - near);
	glm::mat4 projt = { fx / cx, 0.0f, 0.0f, 0.0f,
					   0.0f, fy / cy, 0.0f, 0.0f,
					   0.0f, 0.0f, A, B,
					   0.0f, 0.0f, -1.0f, 0.0f };
	return glm::transpose(projt);
}
