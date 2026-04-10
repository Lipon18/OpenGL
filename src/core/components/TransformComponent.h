#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "BaseComponent.h"

class TransformComponent : public BaseComponent {
public:
	TransformComponent();

	glm::mat4 getModelMatrix() const;

	const glm::vec3& getPosition() const { return m_Position; }
	const glm::vec3& getRotation() const { return m_Rotation; }
	const glm::vec3& getScale() const { return m_Scale; }
	const glm::vec2& getUVScale() const { return m_uvScale; }
	const glm::vec2& getUVOffset() const { return m_uvOffset; }

	void setPosition(const glm::vec3& position) { m_Position = position; }
	void setRotation(const glm::vec3& rotation) { m_Rotation = rotation; }
	void setScale(const glm::vec3& scale) { m_Scale = scale; }

	void setUVScale(const glm::vec2& uvScale) { m_uvScale = uvScale; }
	void setUVOffset(const glm::vec2& uvOffset) { m_uvOffset = uvOffset; }

private:
	// Spatial properties
	glm::vec3 m_Position;
	glm::vec3 m_Rotation; // Euler angles
	glm::vec3 m_Scale;

	// UV properties
	glm::vec2 m_uvScale;
	glm::vec2 m_uvOffset;
};

#endif // !TRANSFORMCOMPONENT_H
