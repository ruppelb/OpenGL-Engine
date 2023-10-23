#pragma once

#include "SceneObject.h"


class SOLine : public SceneObject
{
public:
	SOLine(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer);
	SOLine(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer);

	~SOLine();

	void onRender() override;

	void addUIElements() override;

private:

	void projectMesh();
	int m_selectedCam = 0;
	std::shared_ptr<SceneObject> m_projectedObject;
};

