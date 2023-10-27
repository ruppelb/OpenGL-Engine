#pragma once
#include <string>
#include <iostream>

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "Material.h"

enum MeshType {PNT, PNC,PN,P, EMPTY};
enum RenderType {Default,Lines};

struct VertexP {
    // position
    glm::vec3 Position;

    VertexP(float px, float py, float pz)
        :Position(glm::vec3(px, py, pz))
    {};
    VertexP() {};
};

struct VertexPN : VertexP {
    // normal
    glm::vec3 Normal;
    VertexPN(float px, float py, float pz, float nx, float ny, float nz)
        :VertexP(px, py, pz), Normal(glm::vec3(nx, ny, nz))
    {};
    VertexPN() {};
};

struct VertexPNT :VertexPN {
    // texCoords
    glm::vec2 TexCoords;

    VertexPNT(float px, float py, float pz, float nx, float ny, float nz, float u, float v) 
        :VertexPN(px,py,pz,nx,ny,nz), TexCoords(glm::vec2(u,v))
    {}
    VertexPNT() {};
};

struct VertexPNC :VertexPN {
    //color
    glm::vec4 Color;

    VertexPNC(float px, float py, float pz, float nx, float ny, float nz, float r, float g,float b, float a)
        :VertexPN(px, py, pz, nx, ny, nz), Color(glm::vec4(r,g,b,a))
    {}
    VertexPNC() {};
};

struct VertexUniversal {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 Color;

    /*
    VertexUniversal(VertexP p)
        :Position(p.Position), Normal(glm::vec3(0.)), TexCoords(glm::vec2(0.)), Color(glm::vec4(0.))
    {};
    VertexUniversal(VertexPN pn)
        :Position(pn.Position), Normal(pn.Normal), TexCoords(glm::vec2(0.)), Color(glm::vec4(0.))
    {};
    VertexUniversal(VertexPNT pnt)
        :Position(pnt.Position), Normal(pnt.Normal), TexCoords(pnt.TexCoords), Color(glm::vec4(0.))
    {};
    VertexUniversal(VertexPNC pnc)
        :Position(pnc.Position), Normal(pnc.Normal), Color(pnc.Color), TexCoords(glm::vec2(0.))
    {};
    VertexUniversal() {};*/
};

class Mesh {
public:
    Mesh();
    Mesh(std::vector<VertexPNT> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType = Default);
    Mesh(std::vector<VertexPNC> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType = Default);
    Mesh(std::vector<VertexPN> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType = Default);
    Mesh(std::vector<VertexP> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType = Default);
    void draw(std::shared_ptr<Shader> shader, Renderer* renderer, Material* overrideMaterial = nullptr);

    Mesh(const Mesh& other);

    std::vector<VertexUniversal> getVertices();
    std::vector<unsigned int> getIndices();
    MeshType getType();
    Material getMaterial();

    //bool hasTextures = false;
private:
    //std::vector<std::shared_ptr<Texture>>  textures;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<VertexArray> m_vao;
    MeshType m_type;
    Material m_material;
    RenderType m_renderType;

    //maintain the lists of vertices and indices
    std::vector<VertexUniversal> m_vertices;
    std::vector<unsigned int> m_indices;

    void setupMeshPNT(std::vector<VertexPNT>  vertices, std::vector<unsigned int> indices);
    void setupMeshPNC(std::vector<VertexPNC>  vertices, std::vector<unsigned int> indices);
    void setupMeshPN(std::vector<VertexPN>  vertices, std::vector<unsigned int> indices);
    void setupMeshP(std::vector<VertexP>  vertices, std::vector<unsigned int> indices);
    //void setupTextures(std::shared_ptr<Shader> shader);

};