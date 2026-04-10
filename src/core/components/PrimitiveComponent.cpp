#include "pch.h"
#include "PrimitiveComponent.h"
#include "core/components/TransformComponent.h"
#include "renderer/material/Material.h"
#include "core/mesh/Mesh.h"
#include "renderer/shaders/Shader.h" 

PrimitiveComponent::PrimitiveComponent(PrimitiveType type, std::shared_ptr<Material> material) : m_Type(type), m_Material(material) {
	name = "PrimitiveComponent";
}

void PrimitiveComponent::init() {
	switch (m_Type) {
	case PrimitiveType::E_Cube: createCube();
		break;
	case PrimitiveType::E_Plane: createPlane();
		break;
	case PrimitiveType::E_Cone: createCone();
		break;
	}
}

void PrimitiveComponent::render() {
    if (!m_Mesh || !m_Material || !owner) return;

    auto transform = owner->getComponent<TransformComponent>();
    if (!transform) return;

    m_Material->apply();

    auto shader = m_Material->getShader();
    shader->setMat4("model", transform->getModelMatrix());

    m_Mesh->draw(*shader);
}

void PrimitiveComponent::createCube() {
    std::vector<Vertex> vertices = {
        // positions        // normals       // uvs
        {{-0.5,-0.5,-0.5},{0,0,-1},{0,0}},
        {{ 0.5,-0.5,-0.5},{0,0,-1},{1,0}},
        {{ 0.5, 0.5,-0.5},{0,0,-1},{1,1}},
        {{-0.5, 0.5,-0.5},{0,0,-1},{0,1}},
        // (can expand all 6 faces later)
    };

    std::vector<unsigned int> indices = {
        0,1,2, 2,3,0
    };

    std::vector<Texture> textures;
    m_Mesh = std::make_shared<Mesh>(vertices, indices, textures);
}

void PrimitiveComponent::createPlane() {
    float size = 10.0f;
    std::vector<Vertex> vertices = {
       {{-size, 0.0f, -size}, {0,1,0}, {0,0}},
       {{ size, 0.0f, -size}, {0,1,0}, {1,0}},
       {{ size, 0.0f,  size}, {0,1,0}, {1,1}},
       {{-size, 0.0f,  size}, {0,1,0}, {0,1}}
    };

    std::vector<unsigned int> indices = {
        0,1,2, 2,3,0
    };

    std::vector<Texture> textures;
    m_Mesh = std::make_shared<Mesh>(vertices, indices, textures);
}

void PrimitiveComponent::createCone() {
    const int segments = 20;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back({ {0,0.5f,0},{0,1,0},{0.5,1} }); // tip

    for (int i = 0; i <= segments; i++) {
        float angle = (float)i / segments * 2.0f * 3.14159f;
        float x = cos(angle) * 0.5f;
        float z = sin(angle) * 0.5f;

        vertices.push_back({ {x,-0.5f,z},{x,0,z},{(float)i / segments,0} });
    }

    for (int i = 1; i <= segments; i++) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    std::vector<Texture> textures;
    m_Mesh = std::make_shared<Mesh>(vertices, indices, textures);
}
