#include "GridObject.h"

GridObject::GridObject(std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:m_renderer(renderer)
{
	m_cameraController = CameraController::getInstance();
	m_meshes = meshes;
	m_shader = s;
	m_model = glm::mat4(1.0f);
}

GridObject::GridObject(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:m_renderer(renderer)
{
	m_cameraController = CameraController::getInstance();
	m_meshes = std::vector<std::shared_ptr<Mesh>>{ mesh };
	m_shader = s;
	m_model = glm::mat4(1.0f);
}

GridObject::~GridObject()
{

}

void GridObject::onUpdate(float deltaTime)
{

}

void GridObject::onRender()
{
	//glm::mat4 mvp = m_cameraController->getViewProj() * m_model;
	glm::mat4 vp = m_cameraController->getViewProj();
	glm::mat4 invVP = glm::inverse(vp);
	float nearZ = m_cameraController->getNearZ();
	float farZ = m_cameraController->getFarZ();

	//set shader uniforms
	m_shader->bind();
	m_shader->setUniformMat4f("u_VP", vp);
	m_shader->setUniformMat4f("u_inverseVP", invVP);
	m_shader->setUniform1f("u_nearZ", nearZ);
	m_shader->setUniform1f("u_farZ", farZ);

	for (int i = 0; i < m_meshes.size(); i++) {
		//mesh
		m_meshes[i]->draw(m_shader, m_renderer.get());
	}

}
