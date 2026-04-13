#include "pch.h"
#include "Scene.h"
#include "core/actor/Actor.h"

Scene::~Scene() {
    m_Actors.clear();
}