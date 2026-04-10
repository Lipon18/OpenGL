#ifndef PRIMITIVECOMPONENT_H
#define PRIMITIVECOMPONENT_H

#include "BaseComponent.h"

class Mesh;
class Material;

enum class PrimitiveType {
	E_Cube,
	E_Plane,
	E_Cone
};

class PrimitiveComponent : public BaseComponent {
public:
	PrimitiveComponent(PrimitiveType type, std::shared_ptr<Material> material);

	void init() override;
	void render() override;

private:
	void createCube();
	void createPlane();
	void createCone();

	PrimitiveType m_Type;
	std::shared_ptr<Mesh> m_Mesh;
	std::shared_ptr<Material> m_Material;
};

#endif // !PRIMITIVECOMPONENT_H