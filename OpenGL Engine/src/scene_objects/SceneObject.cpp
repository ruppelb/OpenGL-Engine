#include "SceneObject.h"


SceneObject::SceneObject(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, std::vector<Material> overrideMaterials)
	: m_renderer(renderer),m_scale(glm::vec3(1.0)),m_translation(glm::vec3(0.0)), m_name(name)
{
	m_cameraController = CameraController::getInstance();
	m_lightController = LightController::getInstance();
	m_meshes = meshes;

	m_shader = s;

	m_hidden = false;
	m_renderUI = true;
	m_UISize = ImVec2(420, 100);
	m_model = glm::mat4(1.0f);

	if (!overrideMaterials.empty()) {
		m_overrideMeshMaterials = true;
		m_overrideMaterials = overrideMaterials;
	}

}

SceneObject::SceneObject(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer, Material* overrideMaterial)
	: m_renderer(renderer), m_scale(glm::vec3(1.0)), m_translation(glm::vec3(0.0)), m_name(name)
{
	m_cameraController = CameraController::getInstance();
	m_lightController = LightController::getInstance();
	m_meshes = std::vector<std::shared_ptr<Mesh>>{ mesh };

	m_shader = s;

	m_hidden = false;
	m_renderUI = true;
	m_UISize = ImVec2(420, 100);
	//m_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	m_model = glm::mat4(1.0f);

	if (overrideMaterial != nullptr) {
		m_overrideMeshMaterials = true;
		m_overrideMaterials = std::vector<Material>{ *overrideMaterial};
	}
}

SceneObject::~SceneObject()
{
	
}
//m_markerID(other.m_markerID),m_color(other.m_color),
SceneObject::SceneObject(const SceneObject& other)
	:m_meshes(other.m_meshes), m_shader(other.m_shader), m_overrideMaterials(other.m_overrideMaterials),m_overrideMeshMaterials(other.m_overrideMeshMaterials),m_scale(other.m_scale),
	m_translation(glm::vec3(0.0)),m_cameraController(other.m_cameraController),m_lightController(other.m_lightController), m_renderer(other.m_renderer), m_model(other.m_model),
	m_hidden(other.m_hidden),m_angleX(other.m_angleX), m_angleY(other.m_angleY), m_angleZ(other.m_angleZ), m_renderUI(other.m_renderUI), m_UISize(other.m_UISize)
{
	m_name = other.m_name;
	m_name.append("_copy");
}

void SceneObject::onUpdate(float deltaTime)
{
}

void SceneObject::onRender()
{
	if (!m_hidden) {
		glm::mat4 appliedTransform = glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0), m_translation), glm::radians(m_angleX), glm::vec3(1.0, 0, 0)), glm::radians(m_angleY), glm::vec3(0.0, 1.0, 0.0)), glm::radians(m_angleZ), glm::vec3(0.0, 0.0, 1.0)), m_scale) * m_model;
		glm::mat4 mvp = m_cameraController->getViewProj() * appliedTransform;
		glm::mat4 normalMat = glm::transpose(glm::inverse(glm::rotate(glm::rotate(glm::rotate(m_model, glm::radians(m_angleX), glm::vec3(1.0, 0, 0)), glm::radians(m_angleY), glm::vec3(0.0, 1.0, 0.0)), glm::radians(m_angleZ), glm::vec3(0.0, 0.0, 1.0)))); //glm::scale(m_model,m_scale)

		//set shader uniforms
		m_shader->bind();
		m_shader->setUniformMat4f("u_MVP", mvp);
		m_shader->setUniformMat4f("u_model", appliedTransform);
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
				m_shader->setUniform3f("lights[" + number +"].color", light->color);
				m_shader->setUniform1f("lights[" + number + "].radius", light->radius);
				m_shader->setUniform1f("lights[" + number + "].falloff", light->falloff);
			}
			m_shader->setUniform3f("lights[" + number + "].ambient", ambientLight);
		}

		for (int i = 0; i < m_meshes.size(); i++) {
			//mesh
			if (m_overrideMeshMaterials) {
				m_meshes[i]->draw(m_shader, m_renderer.get(), &m_overrideMaterials[i]);
			}
			else {
				m_meshes[i]->draw(m_shader, m_renderer.get());
			}
		}
	}
}

void SceneObject::onImGuiRender()
{ 
	if (m_renderUI) {
		if (ImGui::CollapsingHeader(m_name.c_str())) {
			ImGui::BeginChild(m_name.c_str(), m_UISize, true);
			ImGui::DragFloat3("Translation ", &m_translation.x, 0.01f, -30.0f, 30.0f);
			ImGui::Separator();
			ImGui::DragFloat("Rotation X ", &m_angleX, 0.1f, -360.f, 360.0f, "%.3f");
			ImGui::DragFloat("Rotation Y ", &m_angleY, 0.1f, -360.f, 360.0f, "%.3f");
			ImGui::DragFloat("Rotation Z ", &m_angleZ, 0.1f, -360.f, 360.0f, "%.3f"); //ImGuiSliderFlags_AlwaysClamp
			addUIElements();
			ImGui::EndChild();
		}
	}
}

void SceneObject::setModelMatrix(glm::mat4 model)
{
	m_model = model;
	m_translation = glm::vec3(0);
	m_angleX = 0.0;
	m_angleY = 0.0;
	m_angleZ = 0.0;
}

glm::mat4 SceneObject::getModelMatrix()
{
	return glm::scale(glm::rotate(glm::rotate(glm::rotate(glm::translate(glm::mat4(1.0), m_translation), glm::radians(m_angleX), glm::vec3(1.0, 0, 0)), glm::radians(m_angleY), glm::vec3(0.0, 1.0, 0.0)), glm::radians(m_angleZ), glm::vec3(0.0, 0.0, 1.0)), m_scale) * m_model;
}

glm::mat4 SceneObject::getModelMatrixForDirections()
{
	return glm::rotate(glm::rotate(glm::rotate(glm::mat4(1.0), glm::radians(m_angleX), glm::vec3(1.0, 0, 0)), glm::radians(m_angleY), glm::vec3(0.0, 1.0, 0.0)), glm::radians(m_angleZ), glm::vec3(0.0, 0.0, 1.0)) * m_model;
}

void SceneObject::setHidden(bool state)
{
	m_hidden = state;
}

void SceneObject::setScale(glm::vec3 scale)
{
	m_scale = scale;
}

void SceneObject::setTranslation(glm::vec3 translation)
{
	m_translation = translation;
}

glm::vec3 SceneObject::getTranslation()
{
	return m_translation;
}

void SceneObject::setRotationX(float angle)
{
	m_angleX = angle;
}

void SceneObject::setRotationY(float angle)
{
	m_angleY = angle;
}

void SceneObject::setRotationZ(float angle)
{
	m_angleZ = angle;
}

bool SceneObject::isHidden()
{
	return m_hidden;
}

void SceneObject::setMesh(std::shared_ptr<Mesh> mesh)
{
	m_meshes = std::vector<std::shared_ptr<Mesh>>{ mesh };
}

void SceneObject::setMeshes(std::vector<std::shared_ptr<Mesh>> meshes)
{
	m_meshes = meshes;
}
