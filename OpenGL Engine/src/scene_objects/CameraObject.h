#pragma once

#include "SceneObject.h"
#include "..\CameraController.h"

class SOCamera : public SceneObject {
public:
	SOCamera(int cameraId, std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, std::vector<Material> overrideMaterials = std::vector<Material>());
	SOCamera(int cameraId, std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, Material* overrideMaterial = nullptr);

	void onUpdate(float deltaTime) override;
	void setFront(glm::vec3 front);
	//void onImGuiRender() override;
private:
	Camera* m_camera;
	int m_cameraId;
};