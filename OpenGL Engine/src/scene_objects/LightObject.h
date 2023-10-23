#pragma once

#include "SceneObject.h"
#include "..\LightController.h"

class SOLight : public SceneObject {
public:
	SOLight(int lightSourceId, std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, std::vector<Material> overrideMaterials = std::vector<Material>());
	SOLight(int lightSourceId, std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, Material* overrideMaterial = nullptr);

	void onUpdate(float deltaTime) override;
	void addUIElements() override;
private:
	LightSource* m_lightSource;
	int m_lightSourceId;
};