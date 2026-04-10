#include "pch.h"
#include "SceneGridComponent.h"
#include "renderer/shaders/Shader.h"

SceneGridComponent::SceneGridComponent(std::shared_ptr<Shader> shader, float spacing, int lineCount) : 
	m_Shader(shader), m_Spacing(spacing), m_LineCount(lineCount) {
	initBuffers();
}

void SceneGridComponent::initBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 4, nullptr, GL_DYNAMIC_DRAW); // initial small buffer

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

void SceneGridComponent::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {
    if (!m_Shader) return;

    m_Shader->activate();
    m_Shader->setMat4("view", view);
    m_Shader->setMat4("projection", projection);

    std::vector<glm::vec3> lines;
    std::vector<glm::vec3> colors;
    float halfSize = m_LineCount * m_Spacing * 0.5f;

    int visibleRange = 50;

    for (int i = -m_LineCount; i <= m_LineCount; ++i) {
        float offset = i * m_Spacing;
        if (std::abs(offset - cameraPos.x) > visibleRange || std::abs(offset - cameraPos.z) > visibleRange) continue;

        float dx = offset - cameraPos.x;
        float dz = offset - cameraPos.z;
        float distance = sqrt(dx * dx + dz * dz); // Euclidean distance
        float alpha = glm::clamp(1.0f - (distance / visibleRange), 0.0f, 1.0f);
        glm::vec3 color = glm::vec3(alpha);

        // Lines along X
        lines.push_back({ offset, 0.0f, -halfSize });
        lines.push_back({ offset, 0.0f, halfSize });

        // Lines along Z
        lines.push_back({ -halfSize, 0.0f, offset });
        lines.push_back({ halfSize, 0.0f, offset });
    }

    for (auto& v : lines) {
        v.x -= fmod(cameraPos.x, m_Spacing);
        v.z -= fmod(cameraPos.z, m_Spacing);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(glm::vec3), lines.data(), GL_DYNAMIC_DRAW);

    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(lines.size()));

    glBindVertexArray(0);
}

