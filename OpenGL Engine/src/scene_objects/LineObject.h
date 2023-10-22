#pragma once

#include "SceneObject.h"


class SOLine : public SceneObject
{
public:
	SOLine(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer);
	SOLine(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer);

	~SOLine();

	void onRender() override;

	virtual void onImGuiRender();

private:
	std::shared_ptr<Mesh> m_projectedMesh;
};

