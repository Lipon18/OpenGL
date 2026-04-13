#ifndef CUBEACTOR_H
#define CUBEACTOR_H

#include "Actor.h"

class Shader;

class CubeActor : public Actor {
public:
	CubeActor(std::shared_ptr<Shader> shader, const std::string& texturePath, float mass = 1.0f, float bounciness = 0.6f);
};

#endif // !CUBEACTOR_H