#include "LightObject.h"

SOLight::SOLight(int lightSourceId, std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer, std::vector<Material> overrideMaterials)
	:SceneObject(name,meshes, s, renderer, overrideMaterials),m_lightSourceId(lightSourceId)
{
	m_lightSource = LightController::getInstance()->getLightSource(lightSourceId);

	//set initial position and rotation according to light source position and rotation

	switch (m_lightSource->type) {
	case Point:
		m_translation = m_lightSource->position;
		break;
	case Directional:
		
		//create rotation matrix from new direction and old direction
		glm::vec3 cross = glm::normalize(glm::cross(m_lightSource->direction, glm::vec3(0.0, 0.0, -1.0)));
		float dot = glm::dot(m_lightSource->direction, glm::vec3(0.0, 0.0, -1.0));
		glm::mat3 skew(0, cross.z, -cross.y, -cross.z, 0, cross.x, cross.y, -cross.x, 0);
		glm::mat3 newRot = glm::mat3(1.0);
		newRot = newRot + skew + (glm::matrixCompMult(skew, skew) * (1 / (1 + dot)));
		glm::mat4 currentPose = getModelMatrix();
		glm::mat4 newPose = glm::mat4(1.0);
		newPose[0] = glm::vec4(newRot[0], currentPose[0][3]);
		newPose[1] = glm::vec4(newRot[1], currentPose[1][3]);
		newPose[2] = glm::vec4(newRot[2], currentPose[2][3]);
		newPose[3] = currentPose[3];
		m_model = (glm::inverse(newPose));

		break;
	}
}

SOLight::SOLight(int lightSourceId, std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer, Material* overrideMaterial)
	:SceneObject(name, mesh, s, renderer, overrideMaterial), m_lightSourceId(lightSourceId)
{
	m_lightSource = LightController::getInstance()->getLightSource(lightSourceId);

	switch (m_lightSource->type) {
	case Point:
		m_translation = m_lightSource->position;
		break;
	case Directional:

		//create rotation matrix from new direction and old direction
		glm::vec3 cross = glm::normalize(glm::cross(m_lightSource->direction, glm::vec3(0.0, 0.0, -1.0)));
		float dot = glm::dot(m_lightSource->direction, glm::vec3(0.0, 0.0, -1.0));
		glm::mat3 skew(0, cross.z, -cross.y, -cross.z, 0, cross.x, cross.y, -cross.x, 0);
		glm::mat3 newRot = glm::mat3(1.0);
		newRot = newRot + skew + (glm::matrixCompMult(skew, skew) * (1 / (1 + dot)));
		glm::mat4 currentPose = getModelMatrix();
		glm::mat4 newPose = glm::mat4(1.0);
		newPose[0] = glm::vec4(newRot[0], currentPose[0][3]);
		newPose[1] = glm::vec4(newRot[1], currentPose[1][3]);
		newPose[2] = glm::vec4(newRot[2], currentPose[2][3]);
		newPose[3] = currentPose[3];
		m_model = (glm::inverse(newPose));

		break;
	}
}

void SOLight::onUpdate(float deltaTime)
{
	SceneObject::onUpdate(deltaTime);

	//align object position with lightsource position
	switch (m_lightSource->type){
	case Point:
		m_lightSource->position = getTranslation();
		break;
	case Directional:
		m_lightSource->direction = glm::normalize(getModelMatrixForDirections() * glm::vec4(0.0, 0.0, -1.0, 1.0));
		break;
	}
}

void SOLight::onImGuiRender()
{
	if (ImGui::CollapsingHeader(m_name.c_str())) {
		
		if (m_lightSource->type == Point) {
			ImGui::BeginChild(m_name.c_str(), ImVec2(420, 155), true);
			ImGui::DragFloat("Radius", &m_lightSource->radius,0.1f, 0.1f, 100);
			ImGui::DragFloat("Falloff", &m_lightSource->falloff, 0.1f,0.1f, 100);
		}
		else {
			ImGui::BeginChild(m_name.c_str(), ImVec2(420, 115), true);
		}
		ImGui::DragFloat4("Color", &m_lightSource->color.x, 0.01f,0.0f, 1.0f);
		ImGui::DragFloat3("Translation ", &m_translation.x, 0.01f, -30.0f, 30.0f);
		ImGui::DragFloat("Rotation X ", &m_angleX, 0.1f, -360.f, 360.0f);
		ImGui::DragFloat("Rotation Y ", &m_angleY, 0.1f, -360.f, 360.0f );
		ImGui::DragFloat("Rotation Z ", &m_angleZ, 0.1f, -360.f, 360.0f);
		ImGui::EndChild();
	}
	
}
