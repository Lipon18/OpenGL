#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "BaseComponent.h"
#include "core/physcis/Collider.h"

class Actor;

class RigidBodyComponent : public BaseComponent{
public:
	RigidBodyComponent();
	~RigidBodyComponent();

	void init() override;
	void update(float deltaTime) override;

	inline void addForce(const glm::vec3& force) { m_Force += force; }
	inline void addTorque(const glm::vec3& torque) { m_Torque += torque; }
	void applyImpulse(const glm::vec3& impulse) { if (m_Mass <= 0.0f) return; m_Velocity += impulse / m_Mass; }

	void setCollider(std::unique_ptr<Collider> collider) { m_Collider = std::move(collider); }
	Collider* getCollider() const { return m_Collider.get(); }

	glm::vec3 getVelocity() const { return m_Velocity; }
	glm::vec3 getAngularVelocity() const { return m_AngularVelocity; }

	float getMass() const { return m_Mass; }
	float getBounciness() const { return m_Bounciness; }
	void setMass(float mass) { m_Mass = mass; }
	void setBounciness(float bounciness) { m_Bounciness = bounciness; }
	Actor* getOwner() const { return owner; }

private:
	float m_Mass = 1.0f;
	float m_Bounciness = 0.5f;
	glm::vec3 m_Force = glm::vec3(0.0f);
	glm::vec3 m_Torque = glm::vec3(0.0f);
	glm::vec3 m_AngularMomentum = glm::vec3(0.0f);
	glm::mat3 m_InverseInertiaTensor = glm::mat3(1.0f);
	glm::vec3 m_Velocity = glm::vec3(0.0f);
	glm::vec3 m_AngularVelocity = glm::vec3(0.0f);

	std::unique_ptr<Collider> m_Collider;
};

#endif // !RIGIDBODYCOMPONENT_H
