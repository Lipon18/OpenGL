#include "pch.h"
#include "Skybox.h"
#include "renderer/skybox/SkyboxData.h"
#include "renderer/shaders/Shader.h"

Skybox::Skybox(const std::vector<std::string>& faces) {
	m_Shader = std::make_shared<Shader>("assets/shaders/skybox_vs.glsl", "assets/shaders/skybox_fs.glsl");
	m_Cubemap = std::make_shared<Cubemap>(faces);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SkyboxData::Vertices), &SkyboxData::Vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void Skybox::render(const glm::mat4& view, const glm::mat4& projection) {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    m_Shader->activate();

    // Remove translation from the view matrix
    glm::mat4 viewMatrix = glm::mat4(glm::mat3(view));

    m_Cubemap->bind(15);
    m_Shader->setInt("skybox", 15);
    m_Shader->setMat4("view", viewMatrix);
    m_Shader->setMat4("projection", projection);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
