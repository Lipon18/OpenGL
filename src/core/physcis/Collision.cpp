#include "pch.h"
#include "Collision.h"
#include "renderer/shaders/Shader.h"
#include "core/components/RigidBodyComponent.h"
#include "core/components/TransformComponent.h"
#include "core/physcis/Collider.h"

unsigned int Collision::m_DebugVAO = 0;
unsigned int Collision::m_DebugVBO = 0;

bool Collision::checkAABB(const AxisAlignedBoundingBox& a, const AxisAlignedBoundingBox& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) && (a.min.y <= b.max.y && a.max.y >= b.min.y) && (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

CollisionManifold Collision::getCollisionManifold(const AxisAlignedBoundingBox& a, const AxisAlignedBoundingBox& b) {
    CollisionManifold manifold;
    manifold.isColliding = false;

    float overlapX = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
    float overlapY = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
    float overlapZ = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);

    // If all overlaps are positive, we have a collision
    if (overlapX > 0 && overlapY > 0 && overlapZ > 0) {
        manifold.isColliding = true;

        // The smallest overlap is our collision axis (The SAT principle)
        if (overlapX < overlapY && overlapX < overlapZ) {
            manifold.penetrationDepth = overlapX;
            manifold.normal = (a.min.x < b.min.x) ? glm::vec3(-1, 0, 0) : glm::vec3(1, 0, 0);
        }
        else if (overlapY < overlapX && overlapY < overlapZ) {
            manifold.penetrationDepth = overlapY;
            manifold.normal = (a.min.y < b.min.y) ? glm::vec3(0, -1, 0) : glm::vec3(0, 1, 0);
        }
        else {
            manifold.penetrationDepth = overlapZ;
            manifold.normal = (a.min.z < b.min.z) ? glm::vec3(0, 0, -1) : glm::vec3(0, 0, 1);
        }
    }

    return manifold;
}

AxisAlignedBoundingBox Collision::getAABB(glm::vec3 position, glm::vec3 scale) {
    return { position - (scale * 0.5f), position + (scale * 0.5f) };
}

void Collision::drawAABB(const AxisAlignedBoundingBox& box, const glm::vec3& color, std::shared_ptr<Shader> debugShader) {
    if (m_DebugVAO == 0)
        initDebugBuffers();

    debugShader->activate();
    debugShader->setVec3("color", color);

    glm::vec3 center = (box.min + box.max) * 0.5f;
    glm::vec3 size = box.max - box.min;

    glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
    model = glm::scale(model, size);
    debugShader->setMat4("model", model);

    glBindVertexArray(m_DebugVAO);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}

void Collision::drawSphere(glm::vec3 center, float radius, const glm::vec3& color, std::shared_ptr<Shader> debugShader) {
    const int segments = 32;

    std::vector<glm::vec3> vertices;

    // XY circle
    for (int i = 0; i < segments; i++) {
        float theta1 = (float)i / segments * 2.0f * glm::pi<float>();
        float theta2 = (float)(i + 1) / segments * 2.0f * glm::pi<float>();

        vertices.push_back(center + glm::vec3(cos(theta1) * radius, sin(theta1) * radius, 0));
        vertices.push_back(center + glm::vec3(cos(theta2) * radius, sin(theta2) * radius, 0));
    }

    // XZ circle
    for (int i = 0; i < segments; i++) {
        float theta1 = (float)i / segments * 2.0f * glm::pi<float>();
        float theta2 = (float)(i + 1) / segments * 2.0f * glm::pi<float>();

        vertices.push_back(center + glm::vec3(cos(theta1) * radius, 0, sin(theta1) * radius));
        vertices.push_back(center + glm::vec3(cos(theta2) * radius, 0, sin(theta2) * radius));
    }

    // YZ circle
    for (int i = 0; i < segments; i++) {
        float theta1 = (float)i / segments * 2.0f * glm::pi<float>();
        float theta2 = (float)(i + 1) / segments * 2.0f * glm::pi<float>();

        vertices.push_back(center + glm::vec3(0, cos(theta1) * radius, sin(theta1) * radius));
        vertices.push_back(center + glm::vec3(0, cos(theta2) * radius, sin(theta2) * radius));
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    debugShader->activate();
    debugShader->setVec3("color", color);

    glm::mat4 model = glm::mat4(1.0f);
    debugShader->setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, (GLsizei)vertices.size());

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void Collision::initDebugBuffers() {
    float vertices[] = {
        -0.5,-0.5,-0.5,  0.5,-0.5,-0.5,  0.5,-0.5,-0.5,  0.5, 0.5,-0.5,
         0.5, 0.5,-0.5, -0.5, 0.5,-0.5, -0.5, 0.5,-0.5, -0.5,-0.5,-0.5,
        -0.5,-0.5, 0.5,  0.5,-0.5, 0.5,  0.5,-0.5, 0.5,  0.5, 0.5, 0.5,
         0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5,-0.5, 0.5,
        -0.5,-0.5,-0.5, -0.5,-0.5, 0.5,  0.5,-0.5,-0.5,  0.5,-0.5, 0.5,
         0.5, 0.5,-0.5,  0.5, 0.5, 0.5, -0.5, 0.5,-0.5, -0.5, 0.5, 0.5
    };

    glGenVertexArrays(1, &m_DebugVAO);
    glGenBuffers(1, &m_DebugVBO);
    glBindVertexArray(m_DebugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_DebugVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Collision::Resolve(RigidBodyComponent* a, RigidBodyComponent* b, const CollisionManifold& manifold) {
    glm::vec3 relativeVelocity = a->getVelocity() - (b ? b->getVelocity() : glm::vec3(0.0f));
    float velAlongNormal = glm::dot(relativeVelocity, manifold.normal);

    if (velAlongNormal > 0.0f) return;

    float e = glm::min(a->getBounciness(), b ? b->getBounciness() : a->getBounciness());

    float invMassA = 1.0f / a->getMass();
    float invMassB = b ? (1.0f / b->getMass()) : 0.0f;
    float invMassSum = invMassA + invMassB;

    float j = -(1 + e) * velAlongNormal;
    //float invMassSum = (1.0f / a->getMass()) + (b ? (1.0f / b->getMass()) : 0.0f);

    j /= invMassSum;

    glm::vec3 impulse = j * manifold.normal;

    a->applyImpulse(impulse);
    if (b) b->applyImpulse(-impulse);

    float percent = 0.2f;
    float slop = 0.01f;
    glm::vec3 correction = std::max(manifold.penetrationDepth - slop, 0.0f) / invMassSum * percent * manifold.normal;

    auto transA = a->getOwner()->getComponent<TransformComponent>();
    if (transA) transA->setPosition(transA->getPosition() + correction * invMassA);

    if (b) {
        auto transB = b->getOwner()->getComponent<TransformComponent>();
        if (transB) transB->setPosition(transB->getPosition() - correction * invMassB);
    }
}

CollisionManifold Collision::checkCollision(RigidBodyComponent* a, TransformComponent* transA, Collider* colA, RigidBodyComponent* b, TransformComponent* transB, Collider* colB) {
    if (colA->getType() == ColliderType::E_Sphere && colB->getType() == ColliderType::E_Sphere) {
        return SphereVsSphere(transA, (SphereCollider*)colA, transB, (SphereCollider*)colB);
    }

    if (colA->getType() == ColliderType::E_Sphere && colB->getType() == ColliderType::E_Box) {
        return SphereVsBox(transA, (SphereCollider*)colA, transB, (BoxCollider*)colB);
    }

    if (colA->getType() == ColliderType::E_Box && colB->getType() == ColliderType::E_Sphere) {
        CollisionManifold m = SphereVsBox(transB, (SphereCollider*)colB, transA, (BoxCollider*)colA);
        m.normal = -m.normal;
        return m;
    }

    if (colA->getType() == ColliderType::E_Box && colB->getType() == ColliderType::E_Box) {
        return BoxVsBox(transA, (BoxCollider*)colA, transB, (BoxCollider*)colB);
    }

    return CollisionManifold();
}

CollisionManifold Collision::SphereVsSphere(TransformComponent* tA, SphereCollider* sA, TransformComponent* tB, SphereCollider* sB) {
    CollisionManifold manifold;
    manifold.isColliding = false;

    glm::vec3 delta = tB->getPosition() - tA->getPosition();
    float distance = glm::length(delta);
    float sumRadius = sA->radius + sB->radius;

    if (distance < sumRadius) {
        manifold.isColliding = true;
        manifold.normal = (distance > 0.0f) ? glm::normalize(delta) : glm::vec3(0, 1, 0);
        manifold.penetrationDepth = sumRadius - distance;
    }
    return manifold;
}

CollisionManifold Collision::SphereVsBox(TransformComponent* tSphere, SphereCollider* s, TransformComponent* tBox, BoxCollider* b) {
    CollisionManifold manifold;
    glm::vec3 sphereCenter = tSphere->getPosition();
    glm::vec3 boxCenter = tBox->getPosition();

    glm::vec3 closestPoint = glm::clamp(sphereCenter, boxCenter - b->halfSize, boxCenter + b->halfSize);

    glm::vec3 delta = sphereCenter - closestPoint;
    float distance = glm::length(delta);

    if (distance < s->radius) {
        manifold.isColliding = true;
        manifold.normal = glm::normalize(delta);
        manifold.penetrationDepth = s->radius - distance;
    }
    return manifold;
}

CollisionManifold Collision::BoxVsBox(TransformComponent* tA, BoxCollider* bA, TransformComponent* tB, BoxCollider* bB) {
    AxisAlignedBoundingBox boxA = { tA->getPosition() - bA->halfSize, tA->getPosition() + bA->halfSize };
    AxisAlignedBoundingBox boxB = { tB->getPosition() - bB->halfSize, tB->getPosition() + bB->halfSize };
    return getCollisionManifold(boxA, boxB);
}