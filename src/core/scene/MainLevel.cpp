#include "pch.h"
#include "MainLevel.h"
#include "renderer/shaders/Shader.h"
#include "renderer/skybox/SkyboxData.h"
#include "renderer/skybox/Skybox.h"
#include "core/components/CameraComponent.h"
#include "core/components/TransformComponent.h"
#include "core/components/RigidBodyComponent.h"
#include "core/physcis/Collision.h"
#include "core/physcis/Collider.h"
#include "core/actor/CubeActor.h"
#include "core/actor/SphereActor.h"
#include "core/actor/PlaneActor.h"
#include "renderer/RenderGlobals.h"
#include "core/actor/Pawn.h"

MainLevel::MainLevel() {}
MainLevel::~MainLevel() {}

void MainLevel::init() {
	srand(static_cast<unsigned int>(time(NULL)));
	auto shader = std::make_shared<Shader>("assets/shaders/model_loading_vs.glsl", "assets/shaders/model_loading_fs.glsl");
	m_Skybox = std::make_shared<Skybox>(SkyboxData::faces);
	auto lineShader = std::make_shared<Shader>("assets/shaders/line_vs.glsl", "assets/shaders/line_fs.glsl");
	m_DebugShader = std::make_shared<Shader>("assets/shaders/debug_vs.glsl", "assets/shaders/debug_fs.glsl");

	setupCamera();

	m_FloorActor = spawnActor<PlaneActor>(shader, "assets/textures/grid/128x128/Gray/Prototype_Grid_Gray_07-128x128.png");

	m_Pawn = spawnActor<Pawn>(shader, "assets/textures/character_diffuse.png");
	m_Pawn->getComponent<TransformComponent>()->setPosition({ 0.0f, 2.0f, 0.0f });
	m_PhysicsActors.push_back(m_Pawn);

	std::string plankTex = "assets/images/plank/Planks012_2K-PNG_Color.png";
	std::string sphereTex = "assets/textures/grid/128x128/Gray/Prototype_Grid_Gray_07-128x128.png";
	for (int i = 0; i < 3; i++) {
		auto cube = spawnActor<CubeActor>(shader, plankTex);
		cube->getComponent<TransformComponent>()->setPosition({ -2.0f + (i * 1.5f), 8.0f + i, 0.0f });
		m_PhysicsActors.push_back(cube);
	}

	for (int i = 0; i < 3; i++) {
		auto sphere = spawnActor<SphereActor>(shader, sphereTex, 0.5f); // 0.5f is radius
		sphere->getComponent<TransformComponent>()->setPosition({ 2.0f + (i * 1.5f), 10.0f + i, 0.0f });
		m_PhysicsActors.push_back(sphere);
	}
}

void MainLevel::update(float deltaTime) {
	if (deltaTime > 0.1f) deltaTime = 0.1f;
	handlePhysics(deltaTime);
	for (auto& actor : m_Actors) actor->updateComponents(deltaTime);
}

void MainLevel::render() {
	for (auto& actor : m_Actors) actor->renderComponents();
	renderDebugCollision();
}

void MainLevel::setupCamera() {
	auto cameraActor = spawnActor();
	auto transform = cameraActor->addComponent<TransformComponent>();
	glm::vec3 spawnPosition = { 0.0f, 4.0f, 8.0f };
	transform->setPosition(spawnPosition);

	m_MainCamera = cameraActor->addComponent<CameraComponent>();
	m_MainCamera->init();
	m_MainCamera->setPosition(spawnPosition);

	m_MainCamera->setPitch(-25.0f);
	m_MainCamera->setYaw(-90.0f);
}

void MainLevel::handlePhysics(float deltaTime) {
	auto floorTransform = m_FloorActor->getComponent<TransformComponent>();
	//auto floorBox = Collision::getAABB(floorTransform->getPosition() - glm::vec3(0, 0.5f, 0), glm::vec3(40.0f, 1.0f, 40.0f));

	for (size_t i = 0; i < m_PhysicsActors.size(); i++) {
		auto actor = m_PhysicsActors[i];
		auto rbActor = actor->getComponent<RigidBodyComponent>();
		auto transformActor = actor->getComponent<TransformComponent>();

		// 1. Gravity
		rbActor->addForce(glm::vec3(0.0f, m_GRAVITY * rbActor->getMass(), 0.0f));

		// 2. Floor Collision
		auto colliderA = rbActor->getCollider();
		BoxCollider floorCollider;
		floorCollider.halfSize = glm::vec3(20.0f, 0.5f, 20.0f);
		auto manifoldFloor = Collision::checkCollision(rbActor, transformActor, colliderA, nullptr, floorTransform, &floorCollider);

		if (manifoldFloor.isColliding) {
			Collision::Resolve(rbActor, nullptr, manifoldFloor);
			transformActor->setPosition(transformActor->getPosition() + manifoldFloor.normal * manifoldFloor.penetrationDepth);
		}

		// 3. Object vs Object Collision (The "Double Loop")
		for (size_t j = i + 1; j < m_PhysicsActors.size(); j++) {
			auto b = m_PhysicsActors[j];

			auto rbB = b->getComponent<RigidBodyComponent>();
			auto transB = b->getComponent<TransformComponent>();

			auto colliderB = rbB->getCollider();

			if (!colliderA || !colliderB) continue;

			auto manifoldOBB = Collision::checkCollision(rbActor, transformActor, colliderA, rbB, transB, colliderB);

			if (manifoldOBB.isColliding) {
				Collision::Resolve(rbActor, rbB, manifoldOBB);

				// Jitter Fix
				float totalInvMass = (1.0f / rbActor->getMass()) + (1.0f / rbB->getMass());
				glm::vec3 correction = (manifoldOBB.penetrationDepth / totalInvMass) * 0.5f * manifoldOBB.normal;
				transformActor->setPosition(transformActor->getPosition() + correction);
				transB->setPosition(transB->getPosition() - correction);
			}
		}
	}
}

void MainLevel::renderDebugCollision() {
	if (!m_ShowDebugCollision) return;

	m_DebugShader->activate();
	m_DebugShader->setMat4("view", gl_View);
	m_DebugShader->setMat4("projection", gl_Projection);

	auto rbFloor = m_FloorActor->getComponent<RigidBodyComponent>();
	auto transFloor = m_FloorActor->getComponent<TransformComponent>();
	if (rbFloor && rbFloor->getCollider()) {
		auto box = static_cast<BoxCollider*>(rbFloor->getCollider());
		Collision::drawAABB({ transFloor->getPosition() - box->halfSize, transFloor->getPosition() + box->halfSize }, glm::vec3(0, 1, 0), m_DebugShader);
	}

	glLineWidth(3.0f);
	for (auto* actor : m_PhysicsActors) {
		auto rb = actor->getComponent<RigidBodyComponent>();
		auto trans = actor->getComponent<TransformComponent>();
		if (!rb || !trans || !rb->getCollider()) continue;

		auto col = rb->getCollider();
		if (col->getType() == ColliderType::E_Sphere) {
			float r = static_cast<SphereCollider*>(col)->radius;
			Collision::drawSphere(trans->getPosition(), r, glm::vec3(0, 1, 0), m_DebugShader);
		}
		else if (col->getType() == ColliderType::E_Capsule) {
			Collision::drawAABB({ trans->getPosition() - glm::vec3(0.5f, 1.0f, 0.5f), trans->getPosition() + glm::vec3(0.5f, 1.0f, 0.5f) }, glm::vec3(0.0f, 1.0f, 1.0f), m_DebugShader);
		}
		else {
			glm::vec3 h = static_cast<BoxCollider*>(col)->halfSize;
			Collision::drawAABB({ trans->getPosition() - h, trans->getPosition() + h }, glm::vec3(0, 1, 0), m_DebugShader); // GREEN
		}
	}
	glLineWidth(1.0f);
}