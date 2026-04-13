#include "pch.h"
#include "SphereActor.h"
#include "renderer/material/Material.h"
#include "core/components/TransformComponent.h"
#include "core/components/PrimitiveComponent.h"
#include "core/components/RigidBodyComponent.h"
#include "core/physcis/Collider.h"
#include "App/TextureLoader.h"

SphereActor::SphereActor(std::shared_ptr<Shader> shader, const std::string& texturePath, float radius, float mass) {
    auto transform = addComponent<TransformComponent>();
    transform->setScale(glm::vec3(radius * 2.0f));

    auto material = std::make_shared<Material>(shader);

    Texture tex = TextureLoader::loadTexture(texturePath);
    material->setTexture(tex.id);

    addComponent<PrimitiveComponent>(PrimitiveType::E_Sphere, material)->init();

    auto rb = addComponent<RigidBodyComponent>();
    rb->setMass(mass);
    rb->setBounciness(0.8f);

    auto collider = std::make_unique<SphereCollider>();
    collider->radius = radius;
    rb->setCollider(std::move(collider));

    rb->init();
}

SphereActor::SphereActor(std::shared_ptr<Shader> shader, const std::string& albedo, const std::string& normal, const std::string& roughness, float radius, float mass) {
 /*   auto transform = addComponent<TransformComponent>();
    transform->setScale(glm::vec3(radius * 2.0f));

    auto material = std::make_shared<Material>(shader);

    Texture albedo = TextureLoader::loadTexture(albedoPath);
    Texture normal = TextureLoader::loadTexture(normalPath);
    Texture roughness = TextureLoader::loadTexture(roughnessPath);

    material->setAlbedo(albedo.id);
    material->setNormal(normal.id);
    material->setRoughness(roughness.id);

    addComponent<PrimitiveComponent>(PrimitiveType::E_Sphere, material)->init();

    auto rb = addComponent<RigidBodyComponent>();
    rb->setMass(mass);
    rb->setBounciness(0.8f);

    auto collider = std::make_unique<SphereCollider>();
    collider->radius = radius;
    rb->setCollider(std::move(collider));

    rb->init();*/
}