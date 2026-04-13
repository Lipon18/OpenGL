#ifndef PAWN_H
#define PAWN_H

#include "Actor.h"

class CameraComponent;
class RigidBodyComponent;
class TransformComponent;
class SpringArmComponent;
class InputComponent;
class Camera;
class Shader;

class Pawn : public Actor {
public:
	Pawn(std::shared_ptr<Shader> shader, const std::string& texturePath);

	void UpdatePawn(float deltaTime);

	CameraComponent* getCameraComponent() const { return m_CameraComp; }

	void MoveForward(float scale);
	void MoveRight(float scale);
	void LookUp(float x, float y);

	void setPossessed(bool possessed) { m_IsPossessed = possessed; }
	bool isPossessed() const { return m_IsPossessed; }

private:
	CameraComponent* m_CameraComp;
	RigidBodyComponent* m_RigidBody;
	TransformComponent* m_Transform;
	SpringArmComponent* m_SpringArm;
	InputComponent* m_Input;

	bool m_IsPossessed = true;
	float m_MoveSpeed = 500.0f;
	glm::vec3 m_CameraOffset = glm::vec3(0.0f, 2.5f, 5.0f);

	void setupPlayerInput();
};

#endif // !PAWN_H