#ifndef MESHCOMPONENT_H
#define MESHCOMPONENT_H

#include "BaseComponent.h"

class Model;
class Material;

class MeshRendererComponent : public BaseComponent {
public:
	MeshRendererComponent(std::shared_ptr<Model> model, std::shared_ptr<Material> material);
	void render() override;

	void setShowDepth(bool value);

private:
	std::shared_ptr<Model> m_Model;
	std::shared_ptr<Material> m_Material;
};

#endif // !MESHCOMPONENT_H