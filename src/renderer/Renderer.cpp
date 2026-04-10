#include "pch.h"
#include "Renderer.h"
#include "renderer/skybox/Skybox.h"
#include "core/components/CameraComponent.h"

bool Renderer::showDepth = false;

void Renderer::DrawSkybox(std::shared_ptr<Skybox>& skybox, CameraComponent* camera, float aspect) {
	if(!skybox || !camera) return;

    glm::mat4 projection = glm::perspective(glm::radians((float)camera->getZoom()), aspect, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();

    skybox->render(view, projection);
}
