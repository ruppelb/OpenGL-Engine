#pragma once

#include "Renderer.h"
#include "CameraController.h"
#include "Mesh.h"
#include <string>
#include <memory>
#include <iostream>

class GridObject
{
public:
	GridObject(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer);
	GridObject(std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer);

	~GridObject();

	void onUpdate(float deltaTime);

	void onRender();

private:
	std::vector<std::shared_ptr<Mesh>> m_meshes;
	std::shared_ptr <Shader> m_shader;
	glm::mat4 m_model;

	CameraController* m_cameraController;
	std::shared_ptr<Renderer> m_renderer;
};

