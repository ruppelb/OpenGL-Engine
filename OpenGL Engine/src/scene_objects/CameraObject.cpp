#include "CameraObject.h"

SOCamera::SOCamera(int cameraId, std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer, std::vector<Material> overrideMaterials)
	:SceneObject(name,meshes, s, renderer, overrideMaterials), m_cameraId(cameraId)
{
	m_camera = CameraController::getInstance()->getCamera(cameraId);
	//set initial position and rotation according to camera position and rotation
	setModelMatrix(glm::inverse(m_camera->view));
}

SOCamera::SOCamera(int cameraId, std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer, Material* overrideMaterial)
	:SceneObject(name, mesh, s, renderer, overrideMaterial), m_cameraId(cameraId)
{
	m_camera = CameraController::getInstance()->getCamera(cameraId);
	//set initial position and rotation according to camera position and rotation
	setModelMatrix(glm::inverse(m_camera->view));
}

void SOCamera::onUpdate(float deltaTime)
{
	SceneObject::onUpdate(deltaTime);

	//check if camera is selected
	if (CameraController::getInstance()->getActiveCam() == m_cameraId) {
		//align object pose with camera pose
		setModelMatrix(glm::inverse(m_camera->view));

		//hide mesh of currently selected camera
		m_hidden = true;
	}
	else {
		//TODO: remove this below
		setModelMatrix(glm::inverse(m_camera->view));
		//TODO align camera matrices with object position
		m_hidden = false;
	}
}
void SOCamera::setFront(glm::vec3 front)
{
	//normalize in case it is not
	front = glm::normalize(front);
	CameraController::getInstance()->setFront(front, m_cameraId);
}
/*
void SOCamera::onImGuiRender()
{
	
	ImGui::BeginChild(m_name.c_str(), ImVec2(450, 150), true);
	ImGui::Text(m_name.c_str());
	if (m_lightSource->type == Point) {
		ImGui::SliderFloat("Radius", &m_lightSource->radius, 0.1f, 100);
		ImGui::SliderFloat("Falloff", &m_lightSource->falloff, 0.1f, 100);
	}
	ImGui::SliderFloat4("Color", &m_lightSource->color.x, 0.0, 1.0);
	ImGui::SliderFloat3("Translation ", &m_translation.x, -30.0f, 30.0f);
	ImGui::SliderFloat("Rotation X ", &m_angleX, 0.f, 6.2831f);
	ImGui::SliderFloat("Rotation Y ", &m_angleY, 0.f, 6.2831f);
	ImGui::SliderFloat("Rotation Z ", &m_angleZ, 0.f, 6.2831f);
	ImGui::EndChild();
}*/
