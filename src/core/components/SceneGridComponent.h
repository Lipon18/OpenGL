#ifndef SCENEGRIDCOMPONENT_H
#define SCENEGRIDCOMPONENT_H

#include "BaseComponent.h"

class Shader;

class SceneGridComponent : public BaseComponent {
public:
	SceneGridComponent(std::shared_ptr<Shader> shader, float spacing = 1.0f, int lineCount = 100);
	void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);

	void setSpacing(float s) { m_Spacing = s; }
	void setLineCount(int count) { m_LineCount = count; }

private:
	std::shared_ptr<Shader> m_Shader;
	float m_Spacing;
	int m_LineCount;

	unsigned int VAO, VBO;
	void initBuffers();
};

#endif // !SCENEGRIDCOMPONENT_H