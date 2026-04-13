#ifndef SPHEREACTOR_H
#define SPHEREACTOR_H

#include "Actor.h"

class Shader;
class Material;

class SphereActor : public Actor {
public:
    SphereActor(std::shared_ptr<Shader> shader, const std::string& texturePath, float radius = 0.5f, float mass = 0.1f);
	SphereActor(std::shared_ptr<Shader> shader, const std::string& albedo, const std::string& normal, const std::string& roughness, float radius = 0.5f, float mass = 0.1f);
};

#endif // !SPHEREACTOR_H