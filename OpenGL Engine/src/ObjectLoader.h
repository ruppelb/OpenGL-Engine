#pragma once
#include<assimp/Importer.hpp> 
#include<assimp/scene.h> 
#include<assimp/postprocess.h>

#include <string>
#include "Renderer.h"
#include "Mesh.h"

class ObjectLoader {
public:
	static ObjectLoader* getInstance();

	std::vector<std::shared_ptr<Mesh>> loadObjectFromFile(const std::string& filepath, MeshType type);
	std::shared_ptr<Mesh> loadCubeVTN(RenderType renderType = Default);
	std::shared_ptr<Mesh> loadPlaneV(RenderType renderType = Default);


	ObjectLoader(ObjectLoader const&) = delete;
	void operator=(ObjectLoader const&) = delete;

	~ObjectLoader();
private:
	static ObjectLoader* inst_;   // The one, single instance

	ObjectLoader() {};

	std::vector<std::shared_ptr<Texture>>textures_loaded;

	std::shared_ptr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, MeshType type);

	std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType t_type);
};