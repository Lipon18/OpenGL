#include "pch.h"
#include "CubeActor.h"
#include "core/components/TransformComponent.h"
#include "core/components/MeshRendererComponent.h"
#include "core/components/RigidBodyComponent.h"
#include "core/physcis/Collider.h"
#include "core/mesh/Model.h"
#include "App/TextureLoader.h"
#include "renderer/material/Material.h"

CubeActor::CubeActor(std::shared_ptr<Shader> shader, const std::string& texturePath, float mass, float bounciness) {
	// 1. Transform
	this->addComponent<TransformComponent>();

    // 2. Mesh & Material
    auto cubeModel = std::make_shared<Model>("assets/models/cube.fbx");
    auto cubeMaterial = std::make_shared<Material>(shader);
    Texture cubeTexture = TextureLoader::loadTexture(texturePath);
    cubeMaterial->setTexture(cubeTexture.id);

    this->addComponent<MeshRendererComponent>(cubeModel, cubeMaterial);

    // 3. Physics & Collider
    auto rb = this->addComponent<RigidBodyComponent>();
    rb->setMass(mass);
    rb->setBounciness(bounciness);

    auto collider = std::make_unique<BoxCollider>();
    collider->halfSize = glm::vec3(0.5f); // Default 1x1x1 cube
    rb->setCollider(std::move(collider));

    rb->init();
}
