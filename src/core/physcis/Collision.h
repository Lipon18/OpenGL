#ifndef COLLISION_H
#define COLLISION_H

class Shader;
class RigidBodyComponent;
class Collider;
class BoxCollider;
class SphereCollider;
class CapsuleCollider;
class TransformComponent;

struct AxisAlignedBoundingBox {
	glm::vec3 min;
	glm::vec3 max;
};

struct CollisionManifold {
	bool isColliding;
	float penetrationDepth;
	glm::vec3 normal;

	CollisionManifold() : isColliding(false), penetrationDepth(0.0f), normal(glm::vec3(0.0f)) {}
};

class Collision {
public:
	static bool checkAABB(const AxisAlignedBoundingBox& a, const AxisAlignedBoundingBox& b);
	static CollisionManifold getCollisionManifold(const AxisAlignedBoundingBox& a, const AxisAlignedBoundingBox& b);
	static AxisAlignedBoundingBox getAABB(glm::vec3 position, glm::vec3 scale);
	static void drawAABB(const AxisAlignedBoundingBox& box, const glm::vec3& color, std::shared_ptr<Shader> debugShader);
	static void drawSphere(glm::vec3 center, float radius, const glm::vec3& color, std::shared_ptr<Shader> debugShader);
	static void Resolve(RigidBodyComponent* a, RigidBodyComponent* b, const CollisionManifold& manifold);
	static CollisionManifold checkCollision(RigidBodyComponent* a, TransformComponent* transA, Collider* colA, RigidBodyComponent* b, TransformComponent* transB, Collider* colB);

private:
	static unsigned int m_DebugVAO, m_DebugVBO;
	static void initDebugBuffers();

	static CollisionManifold SphereVsSphere(TransformComponent* tA, SphereCollider* sA, TransformComponent* tB, SphereCollider* sB);
	static CollisionManifold SphereVsBox(TransformComponent* tSphere, SphereCollider* s, TransformComponent* tBox, BoxCollider* b);
	static CollisionManifold BoxVsBox(TransformComponent* tA, BoxCollider* bA, TransformComponent* tB, BoxCollider* bB);
};

#endif // !COLLISION_H