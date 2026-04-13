#include "pch.h"
#include "PlaneActor.h"
#include "core/components/TransformComponent.h"
#include "core/components/PrimitiveComponent.h"
#include "App/TextureLoader.h"
#include "renderer/material/Material.h"

PlaneActor::PlaneActor(std::shared_ptr<Shader> shader, const std::string& texturePath) {
    this->addComponent<TransformComponent>();

    auto floorMaterial = std::make_shared<Material>(shader);
    Texture floorTexture = TextureLoader::loadTexture(texturePath);
    floorMaterial->setTexture(floorTexture.id);

    auto floorPrimitive = this->addComponent<PrimitiveComponent>(PrimitiveType::E_Plane, floorMaterial);
    floorPrimitive->init();
}
