#include "LineObject.h"

SOLine::SOLine(std::string name, std::vector<std::shared_ptr<Mesh>> meshes, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:SceneObject(name, meshes, s, renderer)
{
	m_projectedMesh = std::make_shared<Mesh>();
}

SOLine::SOLine(std::string name, std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> s, std::shared_ptr<Renderer> renderer)
	:SceneObject(name, mesh,s,renderer)
{
	m_projectedMesh = std::make_shared<Mesh>();
}

SOLine::~SOLine()
{
}

void SOLine::onRender()
{
	//render original mesh first
	SceneObject::onRender();

	//render projected mesh

}

void SOLine::onImGuiRender()
{
	//added just for testing
	SceneObject::onImGuiRender();
	//add button for projection
}
