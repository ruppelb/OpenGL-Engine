#pragma once

#include "../Renderer.h"
#include "../Texture.h"
#include "../Material.h"
#include "../CameraController.h"
#include "../LightController.h"
#include "../Mesh.h"
#include <string>
#include <memory>
#include <iostream>
#include <imgui/imgui.h>

class SceneObject {
public:
	SceneObject(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, std::vector<Material> overrideMaterials = std::vector<Material>());
	SceneObject(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, Material* overrideMaterial = nullptr);
	
	~SceneObject();


	SceneObject(const SceneObject& other);


	virtual void onUpdate(float deltaTime);

	virtual void onRender();

	virtual void onImGuiRender();

	void setModelMatrix(glm::mat4 model);
	glm::mat4 getModelMatrix();
	glm::mat4 getModelMatrixForNormals();

	void setHidden(bool state);

	void setScale(glm::vec3 scale);

	void setTranslation(glm::vec3 translation);
	glm::vec3 getTranslation();

	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);

	bool isHidden();

protected:

	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::shared_ptr <Shader> m_shader;
	glm::mat4 m_model;
	std::vector<Material> m_overrideMaterials;
	glm::vec3 m_scale;
	glm::vec3 m_translation;
	float m_angleX = 0.0;
	float m_angleY = 0.0;
	float m_angleZ = 0.0;
	bool m_overrideMeshMaterials = false;

	CameraController* m_cameraController;
	LightController* m_lightController;
	std::shared_ptr<Renderer> m_renderer;
	bool m_hidden;
	std::string m_name;
};