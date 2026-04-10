#include "pch.h"
#include "Scene.h"
#include "core/actor/Actor.h"

Scene::~Scene() {
    m_Actors.clear();
}

Actor* Scene::spawnActor() {
    m_Actors.emplace_back(std::make_unique<Actor>());
    return m_Actors.back().get();
}