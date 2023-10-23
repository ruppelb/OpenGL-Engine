#include "LineObject.h"

SOLine::SOLine(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:SceneObject(name, meshes, s, renderer)
{
	m_projectedMesh = std::make_shared<Mesh>();
	name = name.append("_projection");
	m_projectedObject = std::make_shared<SceneObject>(name,m_projectedMesh,s,renderer);

	m_UISize = ImVec2(420, 155);
}

SOLine::SOLine(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:SceneObject(name, mesh,s,renderer)
{
	m_projectedMesh = std::make_shared<Mesh>();
	name = name.append("_projection");
	m_projectedObject = std::make_shared<SceneObject>(name, m_projectedMesh, s, renderer);

	m_UISize = ImVec2(420, 155);
}

SOLine::~SOLine()
{
}

void SOLine::onRender()
{
	//render original mesh first
	SceneObject::onRender();

	m_projectedObject->onRender();
	/*
	//render projected mesh
	//Could be theoretically implemented using a sceneObject as "childObject"
	glm::mat4 mvp = m_cameraController->getViewProj() * m_projectedMeshModel;
	glm::mat4 normalMat = glm::transpose(glm::inverse(m_projectedMeshModel));
	
	//set shader uniforms
	m_shader->bind();
	m_shader->setUniformMat4f("u_MVP", mvp);
	m_shader->setUniformMat4f("u_model", m_projectedMeshModel);
	m_shader->setUniformMat4f("u_normalMat", normalMat);
	
	//set uniforms for lighting
	glm::vec3 viewPos = glm::inverse(m_cameraController->getView())[3];
	m_shader->setUniform3f("view_pos", viewPos);
	std::vector<LightSource*> lights = m_lightController->getLightSources();
	glm::vec3 ambientLight = m_lightController->getAmbientLight();

	for (int i = 0; i < lights.size(); i++) {
		LightSource* light = lights[i];
		std::string number = std::to_string(i);
		//std::cout << glm::to_string(light->position) << std::endl;
		if (!light->hidden) {
			if (light->type == Directional) {
				m_shader->setUniform4f("lights[" + number + "].vector", light->direction.x, light->direction.y, light->direction.z, 0.0);
			}
			else {
				m_shader->setUniform4f("lights[" + number + "].vector", light->position.x, light->position.y, light->position.z, 1.0);
			}
			m_shader->setUniform3f("lights[" + number + "].color", light->color);
			m_shader->setUniform1f("lights[" + number + "].radius", light->radius);
			m_shader->setUniform1f("lights[" + number + "].falloff", light->falloff);
		}
		m_shader->setUniform3f("lights[" + number + "].ambient", ambientLight);
	}

	m_projectedMesh->draw(m_shader, m_renderer.get());*/
}

void SOLine::addUIElements()
{
	ImGui::Separator();
	ImGui::Text("Project to Camera");
	ImGui::DragInt("Cam", &m_selectedCam, 1.0f, 0, CameraController::getInstance()->getCameraCount() - 1, "%d", ImGuiSliderFlags_AlwaysClamp);
	if (ImGui::Button("Project Mesh")) {
		projectMesh();
	}
	m_projectedObject->onImGuiRender();
}

void SOLine::projectMesh()
{
	m_projectedObject->setMeshes(m_meshes);
}
