#include "pch.h"
#include "PrimitiveComponent.h"
#include "core/components/TransformComponent.h"
#include "renderer/material/Material.h"
#include "core/mesh/Mesh.h"
#include "renderer/shaders/Shader.h" 
#include "renderer/Renderer.h"

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
    case PrimitiveType::E_Sphere: createSphere();
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
    shader->setMat4("view", gl_View);
    shader->setMat4("projection", gl_Projection);
    shader->setVec2("uvScale", transform->getUVScale());
    shader->setVec2("uvOffset", transform->getUVOffset());
    shader->setBool("showDepth", Renderer::getShowDepth());

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

void PrimitiveComponent::createSphere() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    const unsigned int X_SEGMENTS = 32;
    const unsigned int Y_SEGMENTS = 32;
    const float PI = 3.14159265359f;

    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            vertices.push_back({ {xPos * 0.5f, yPos * 0.5f, zPos * 0.5f}, {xPos, yPos, zPos}, {xSegment, ySegment} });
        }
    }

    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            indices.push_back(y * (X_SEGMENTS + 1) + x);
            indices.push_back(y * (X_SEGMENTS + 1) + x + 1);
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            indices.push_back(y * (X_SEGMENTS + 1) + x + 1);
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x + 1);
        }
    }
    m_Mesh = std::make_shared<Mesh>(vertices, indices, std::vector<Texture>());
}
