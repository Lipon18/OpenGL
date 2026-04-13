#include "pch.h"
#include "Pawn.h"
#include "core/components/TransformComponent.h"
#include "core/components/MeshRendererComponent.h"
#include "core/components/RigidBodyComponent.h"
#include "core/components/CameraComponent.h"
#include "core/components/SpringArmComponent.h"
#include "core/components/InputComponent.h"
#include "core/mesh/Model.h"
#include "App/TextureLoader.h"
#include "renderer/material/Material.h"

Pawn::Pawn(std::shared_ptr<Shader> shader, const std::string& texturePath) {
    m_Transform = this->addComponent<TransformComponent>();
    m_SpringArm = addComponent<SpringArmComponent>();

    //Physics
    m_RigidBody = this->addComponent<RigidBodyComponent>();
    auto capsule = std::make_unique<CapsuleCollider>();
    capsule->radius = 0.5f;
    capsule->height = 2.0f;
    m_RigidBody->setCollider(std::move(capsule));
    m_RigidBody->setMass(70.0f); // Human-like mass

    //Mesh
    auto pawnModel = std::make_shared<Model>("assets/models/character.fbx"); // or cube.fbx
    auto pawnMaterial = std::make_shared<Material>(shader);
    pawnMaterial->setTexture(TextureLoader::loadTexture(texturePath).id);
    this->addComponent<MeshRendererComponent>(pawnModel, pawnMaterial);

    // 4. 3rd Person Camera
    m_CameraComp = this->addComponent<CameraComponent>(); 
    //m_CameraComp->setRelativeOffset(m_CameraOffset);

    m_Input = addComponent<InputComponent>();
    setupPlayerInput();
}

void Pawn::UpdatePawn(float deltaTime) {
    if (!m_IsPossessed) return;
    m_CameraComp->setPosition(m_SpringArm->getPosition());
}

void Pawn::setupPlayerInput() {
    m_Input->mapKey("PawnMoveForward", GLFW_KEY_W, 1.0f);
    m_Input->mapKey("PawnMoveForward", GLFW_KEY_S, -1.0f);
    m_Input->mapKey("PawnMoveRight", GLFW_KEY_D, 1.0f);
    m_Input->mapKey("PawnMoveRight", GLFW_KEY_A, -1.0f);

    m_Input->bindAction({ "PawnMoveForward", E_InputType::E_Axis1D, E_InputTrigger::E_Held, [this](const InputValue& v) {
         if (m_IsPossessed) this->MoveForward(v.getFloat());
     } });

    m_Input->bindAction({ "PawnMoveRight", E_InputType::E_Axis1D, E_InputTrigger::E_Held, [this](const InputValue& v) {
        if (m_IsPossessed) this->MoveRight(v.getFloat());
    } });
}

void Pawn::MoveForward(float scale) {
    if (m_RigidBody && std::abs(scale) > 0.1f) {
        glm::vec3 forward = glm::vec3(0, 0, -1);
        m_RigidBody->addForce(forward * scale * m_MoveSpeed);
    }
}

void Pawn::MoveRight(float scale) {
    if (m_RigidBody && std::abs(scale) > 0.1f) {
        glm::vec3 right = glm::vec3(1, 0, 0);
        m_RigidBody->addForce(right * scale * m_MoveSpeed);
    }
}

void Pawn::LookUp(float x, float y) {
    m_CameraComp->processMouseMovement(x, y);
}