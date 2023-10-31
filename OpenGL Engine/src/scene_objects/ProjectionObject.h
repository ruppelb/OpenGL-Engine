#pragma once

#include "SceneObject.h"

class SOProjection : public SceneObject
{
public:
	SOProjection(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, RenderType type = Default);
	SOProjection(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr <Shader>s, std::shared_ptr<Renderer> renderer, RenderType type = Default);

	~SOProjection();

	void onRender() override;

	void addUIElements() override;

private:

	void projectMesh();
	int m_selectedCam = 0;
	RenderType m_renderType;
	const char* m_selectedRenderType;
	std::shared_ptr<SceneObject> m_projectedObject;
};

