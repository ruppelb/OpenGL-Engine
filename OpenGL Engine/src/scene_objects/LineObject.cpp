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
	//m_projectedObject->setMeshes(m_meshes);

	std::vector< std::shared_ptr<Mesh>> resultMeshes;
	glm::mat4 view = CameraController::getInstance()->getView(m_selectedCam+1);
	glm::mat4 proj = CameraController::getInstance()->getProj(m_selectedCam+1);
	//project every mesh of parent line object
	for (int i = 0; i < m_meshes.size(); i++) {
		std::vector<VertexUniversal> vertices = m_meshes[i]->getVertices();
		std::vector<unsigned int> indices = m_meshes[i]->getIndices();
		MeshType t = m_meshes[i]->getType();
		Material mat = m_meshes[i]->getMaterial();

		switch (t)
		{
		case PNT:
			{
				std::vector<VertexPNT> resultVertices;

				for (int j = 0; j < vertices.size(); j++) {
					VertexPNT resultVertex;

					//transform position
					glm::vec4 transformedPoint = proj * view * getModelMatrix() * glm::vec4(vertices[j].Position,1.0);
					//std::cout << "Transformed \n" << glm::to_string(transformedPoint) << std::endl;
					//representation of 2D projected point (z coordinate does not need to be one, since it is clamped to 0.0 1.0 range and only used for depth estimation. Other coordinates get linearly mapped to screen coordinates)
					glm::vec3 projectedPoint = transformedPoint / transformedPoint.w;
					
					//std::cout << "Projected \n"<< glm::to_string(projectedPoint )<< std::endl;
					
					//representation of 2D poin in 3D (we have to backproject into 3D space)
					glm::vec4 backprojectedPoint = glm::inverse(proj) * glm::vec4(projectedPoint,1.0);
					
					//std::cout << "Backprojected \n" << glm::to_string(backprojectedPoint) << std::endl;

					resultVertex.Position = glm::vec3(backprojectedPoint);

					//transform normal (leave the same so that lighting changes on original mesh are reflected on projected mesh)
					resultVertex.Normal = vertices[j].Normal;

					//apply texture coordinate
					resultVertex.TexCoords = vertices[j].TexCoords;

					//push back vertex
					resultVertices.push_back(resultVertex);
				}

				std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>(resultVertices, indices, mat,Lines);
				resultMeshes.push_back(resultMesh);
			}
			break;
		case PNC:
		{
			std::vector<VertexPNC> resultVertices;

			for (int j = 0; j < vertices.size(); j++) {
				VertexPNC resultVertex;

				glm::vec4 transformedPoint = proj * view * getModelMatrix() * glm::vec4(vertices[j].Position, 1.0);
				glm::vec3 projectedPoint = transformedPoint / transformedPoint.w;
				glm::vec4 backprojectedPoint = glm::inverse(proj) * glm::vec4(projectedPoint, 1.0);
				resultVertex.Position = glm::vec3(backprojectedPoint);
				resultVertex.Normal = vertices[j].Normal;
				resultVertex.Color = vertices[j].Color;

				resultVertices.push_back(resultVertex);
			}

			std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>(resultVertices, indices, mat, Lines);
			resultMeshes.push_back(resultMesh);
		}
			break;
		case PN:
		{
			std::vector<VertexPN> resultVertices;

			for (int j = 0; j < vertices.size(); j++) {
				VertexPN resultVertex;

				glm::vec4 transformedPoint = proj * view * getModelMatrix() * glm::vec4(vertices[j].Position, 1.0);
				glm::vec3 projectedPoint = transformedPoint / transformedPoint.w;
				glm::vec4 backprojectedPoint = glm::inverse(proj) * glm::vec4(projectedPoint, 1.0);
				resultVertex.Position = glm::vec3(backprojectedPoint);
				resultVertex.Normal = vertices[j].Normal;

				resultVertices.push_back(resultVertex);
			}

			std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>(resultVertices, indices, mat, Lines);
			resultMeshes.push_back(resultMesh);
		}
			break;
		case P:
		{
			std::vector<VertexP> resultVertices;

			for (int j = 0; j < vertices.size(); j++) {
				VertexP resultVertex;

				glm::vec4 transformedPoint = proj * view * getModelMatrix() * glm::vec4(vertices[j].Position, 1.0);
				glm::vec3 projectedPoint = transformedPoint / transformedPoint.w;
				glm::vec4 backprojectedPoint = glm::inverse(proj) * glm::vec4(projectedPoint, 1.0);
				resultVertex.Position = glm::vec3(backprojectedPoint);

				resultVertices.push_back(resultVertex);
			}

			std::shared_ptr<Mesh> resultMesh = std::make_shared<Mesh>(resultVertices, indices, mat, Lines);
			resultMeshes.push_back(resultMesh);
		}
			break;
		case EMPTY:
			break;
		default:
			break;
		}
	}
	

	//add inverse camera matrix of projectioncam as model matrix to m_projectObject
	m_projectedObject->setModelMatrix(glm::inverse(view));
	//set mesh to projected mesh
	m_projectedObject->setMeshes(resultMeshes);
}
