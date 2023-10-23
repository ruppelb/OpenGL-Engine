#include "LineObject.h"

SOLine::SOLine(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:SceneObject(name, meshes, s, renderer)
{
	name = name.append("_projection");
	m_projectedObject = std::make_shared<SceneObject>(name, std::make_shared<Mesh>(),s,renderer);

	m_UISize = ImVec2(420, 155);
}

SOLine::SOLine(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:SceneObject(name, mesh,s,renderer)
{
	name = name.append("_projection");
	m_projectedObject = std::make_shared<SceneObject>(name, std::make_shared<Mesh>(), s, renderer);

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
	std::vector< std::shared_ptr<Mesh>> resultMeshes;
	glm::mat4 view = CameraController::getInstance()->getView(m_selectedCam);
	glm::mat4 proj = CameraController::getInstance()->getProj(m_selectedCam);

	//project every mesh of parent line object
	for (int i = 0; i < m_meshes.size(); i++) {
		std::vector<VertexPNT> resultVertices;


		m_projectedObject->setMeshes(m_meshes);

		//TODO: alter mesh class so that it retains original vertex and index lists. Make vertex hierarchical?
		//std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>(resultVertices, m_meshes[i]);
		//resultMeshes.push_back(resultMesh);
	}

	//add inverse camera matrix of projectioncam as model matrix to m_projectObject
	m_projectedObject->setModelMatrix(glm::inverse(view));
	//set mesh to projected mesh
	m_projectedObject->setMeshes(resultMeshes);
}
