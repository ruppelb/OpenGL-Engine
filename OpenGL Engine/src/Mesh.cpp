#include "Mesh.h"

Mesh::Mesh()
    :m_type(EMPTY),m_material(Material(0.0)), m_renderType(Default)
{
}

Mesh::Mesh(std::vector<VertexPNT> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType)
    :m_type(PNT), m_material(material), m_renderType(renderType),m_indices(indices)
{
    //save input params
    for (int i = 0; i < vertices.size(); i++) {
        VertexUniversal v;
        v.Position = vertices[i].Position;
        v.Normal = vertices[i].Normal;
        v.TexCoords = vertices[i].TexCoords;
        m_vertices.push_back(v);
    }

    setupMeshPNT(vertices, indices);
}

Mesh::Mesh(std::vector<VertexPNC> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType)
    :m_type(PNC), m_material(material), m_renderType(renderType), m_indices(indices)
{
    //save input params
    for (int i = 0; i < vertices.size(); i++) {
        VertexUniversal v;
        v.Position = vertices[i].Position;
        v.Normal = vertices[i].Normal;
        v.Color = vertices[i].Color;
        m_vertices.push_back(v);
    }

    setupMeshPNC(vertices, indices);
}


Mesh::Mesh(std::vector<VertexPN> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType)
    :m_type(PN), m_material(material), m_renderType(renderType), m_indices(indices)
{
    //save input params
    for (int i = 0; i < vertices.size(); i++) {
        VertexUniversal v;
        v.Position = vertices[i].Position;
        v.Normal = vertices[i].Normal;
        m_vertices.push_back(v);
    }

    setupMeshPN(vertices,indices);
}

Mesh::Mesh(std::vector<VertexP> vertices, std::vector<unsigned int> indices, Material material, RenderType renderType)
    :m_type(P), m_material(material), m_renderType(renderType), m_indices(indices)
{
    //save input params
    for (int i = 0; i < vertices.size(); i++) {
        VertexUniversal v;
        v.Position = vertices[i].Position;
        m_vertices.push_back(v);
    }

    setupMeshP(vertices, indices);
}

void Mesh::draw(std::shared_ptr<Shader> shader, Renderer* renderer, Material* overrideMaterial)
{
    if (m_type != EMPTY) {
        if (overrideMaterial == nullptr) {
            m_material.setMaterialUniforms(shader);
        }
        else {
            overrideMaterial->setMaterialUniforms(shader);
        }

        switch (m_renderType)
        {
        case Default:
            //render vertices as triangles
            renderer->draw(*m_vao, *m_indexBuffer, *shader);
            break;
        case Lines:
            //render vertices as lines
            renderer->drawLines(*m_vao, *m_indexBuffer, *shader);
            break;
        default:
            break;
        }
    }
}

Mesh::Mesh(const Mesh& other)
    :m_indexBuffer(other.m_indexBuffer),m_vertexBuffer(other.m_vertexBuffer),m_vao(other.m_vao), m_type(other.m_type), m_material(other.m_material), m_renderType(other.m_renderType)
{
}

std::vector<VertexUniversal> Mesh::getVertices()
{
    return m_vertices;
}

std::vector<unsigned int> Mesh::getIndices()
{
    return m_indices;
}

MeshType Mesh::getType()
{
    return m_type;
}

Material Mesh::getMaterial()
{
    return m_material;
}

void Mesh::setupMeshPNT(std::vector<VertexPNT>  vertices, std::vector<unsigned int> indices)
{
    m_vao = std::make_shared<VertexArray>();
    m_vertexBuffer = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * sizeof(VertexPNT));

    VertexBufferLayout layout;
    layout.push<float>(3); //positions
    layout.push<float>(3); //normals
    layout.push<float>(2); //texture coordinates
    m_vao->addBuffer(*m_vertexBuffer, layout);

    m_indexBuffer = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

void Mesh::setupMeshPNC(std::vector<VertexPNC> vertices, std::vector<unsigned int> indices)
{
    m_vao = std::make_shared<VertexArray>();
    m_vertexBuffer = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * sizeof(VertexPNC));

    VertexBufferLayout layout;
    layout.push<float>(3); //positions
    layout.push<float>(3); //normals
    layout.push<float>(4); //colors
    m_vao->addBuffer(*m_vertexBuffer, layout);

    m_indexBuffer = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

void Mesh::setupMeshPN(std::vector<VertexPN>  vertices, std::vector<unsigned int> indices)
{
    GLCall(m_vao = std::make_shared<VertexArray>());
    GLCall(m_vertexBuffer = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * sizeof(VertexPN)));

    VertexBufferLayout layout;
    GLCall(layout.push<float>(3)); //positions
    GLCall(layout.push<float>(3)); //normals
    m_vao->addBuffer(*m_vertexBuffer, layout);
    GLCall(m_indexBuffer = std::make_shared<IndexBuffer>(&indices[0], indices.size()));
}

void Mesh::setupMeshP(std::vector<VertexP> vertices, std::vector<unsigned int> indices)
{
    m_vao = std::make_shared<VertexArray>();
    m_vertexBuffer = std::make_shared<VertexBuffer>(&vertices[0], vertices.size() * sizeof(VertexP));

    VertexBufferLayout layout;
    layout.push<float>(3); //positions
    m_vao->addBuffer(*m_vertexBuffer, layout);

    m_indexBuffer = std::make_shared<IndexBuffer>(&indices[0], indices.size());
}

/*
void Mesh::setupTextures(std::shared_ptr<Shader> shader) {
    unsigned int diffuseNr = 0;
    unsigned int specularNr = 0;
    unsigned int noneNr = 0;

    for (int i = 0; i < textures.size(); i++) {
        textures[i]->bind(i);

        std::string number;
        std::string name;
        switch (textures[i]->m_type) {
        case diffuse:
            name = "material.texture_d";
            number = std::to_string(diffuseNr++);
            break;
        case specular:
            name = "material.texture_s";
            number = std::to_string(specularNr++);
            break;
        case None:
            name = "texture";
            number = std::to_string(noneNr++);
            break;
        default:
            std::cout << "Setting texture uniforms failed: Unknown texture type.";
            continue;
        }
        shader->setUniform1i((name + number), i);
    }
}*/
