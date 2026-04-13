#ifndef PLANEACTOR_H
#define PLANEACTOR_H

#include "Actor.h"

class Shader;

class PlaneActor : public Actor {
public:
	PlaneActor(std::shared_ptr<Shader> shader, const std::string& texturePath);
};

#endif // !1