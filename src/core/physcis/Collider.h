#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>

enum class ColliderType {
	E_Box,
	E_Sphere,
	E_Capsule
};

class Collider {
public:
	virtual ~Collider() = default;
	virtual ColliderType getType() const = 0;
};

class BoxCollider : public Collider {
public:
	glm::vec3 halfSize;
	ColliderType getType() const override { return ColliderType::E_Box; }
};

class SphereCollider : public Collider {
public:
	float radius;
	ColliderType getType() const override { return ColliderType::E_Sphere; }
};

class CapsuleCollider : public Collider {
public:
	float radius;
	float height;
	ColliderType getType() const override { return ColliderType::E_Capsule; }
};

#endif // !COLLIDER_H

