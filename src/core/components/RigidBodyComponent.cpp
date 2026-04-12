#include "pch.h"
#include "RigidBodyComponent.h"
#include "core/components/TransformComponent.h"

RigidBodyComponent::RigidBodyComponent() = default;
RigidBodyComponent::~RigidBodyComponent() = default;

void RigidBodyComponent::init() {
    float w = 1.0f, h = 1.0f, d = 1.0f;

    glm::mat3 inertiaTensor(0.0f);
    inertiaTensor[0][0] = (1.0f / 12.0f) * m_Mass * (h * h + d * d);
    inertiaTensor[1][1] = (1.0f / 12.0f) * m_Mass * (w * w + d * d);
    inertiaTensor[2][2] = (1.0f / 12.0f) * m_Mass * (w * w + h * h);

    m_InverseInertiaTensor = glm::inverse(inertiaTensor);
}

void RigidBodyComponent::update(float deltaTime) {
	auto transformComponent = owner->getComponent<TransformComponent>();
	if (!transformComponent || m_Mass <= 0.0f) return;

    // 1. Linear Physics (Movement)
    glm::vec3 acceleration = m_Force / m_Mass;
    m_Velocity += acceleration * deltaTime;
    transformComponent->setPosition(transformComponent->getPosition() + m_Velocity * deltaTime);

    // 2. Angular Physics (Rotation)
    // In Hecker's code: Target.AngularMomentum = Source.AngularMomentum + DeltaTime * Source.Torque;
    m_AngularMomentum += m_Torque * deltaTime;

    // Update Angular Velocity based on the Inertia Tensor
    m_AngularVelocity = m_InverseInertiaTensor * m_AngularMomentum;

    // Update the actual rotation of the object
    glm::vec3 currentRot = transformComponent->getRotation();
    currentRot += m_AngularVelocity * deltaTime;
    transformComponent->setRotation(currentRot);

    // 3. Reset Forces for next frame
    m_Force = glm::vec3(0.0f);
    m_Torque = glm::vec3(0.0f);
}