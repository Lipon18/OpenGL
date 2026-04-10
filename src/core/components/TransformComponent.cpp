#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent() : m_Position(0.0f), m_Rotation(0.0f), m_Scale(1.0f), m_uvScale(1.0f), m_uvOffset(0.0f){
    name = "TransformComponent";
}

glm::mat4 TransformComponent::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Position);
    model = glm::rotate(model, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
    model = glm::scale(model, m_Scale);
    return model;
}