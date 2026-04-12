#include "pch.h"
#include "MainLevel.h"
#include "core/mesh/Model.h"
#include "core/mesh/Mesh.h"
#include "App/TextureLoader.h"
#include "renderer/material/Material.h"
#include "renderer/shaders/Shader.h"
#include "core/components/CameraComponent.h"
#include "core/components/TransformComponent.h"
#include "core/components/MeshRendererComponent.h"
#include "core/components/PrimitiveComponent.h"
#include "core/components/RigidBodyComponent.h"
#include "renderer/skybox/SkyboxData.h"
#include "renderer/skybox/Skybox.h"
#include "core/physcis/Collision.h"
#include "core/physcis/Collider.h"

MainLevel::MainLevel() {}
MainLevel::~MainLevel() {}

void MainLevel::init() {
	srand(static_cast<unsigned int>(time(NULL)));
	auto lineShader = std::make_shared<Shader>("assets/shaders/line_vs.glsl", "assets/shaders/line_fs.glsl");
	auto shader = std::make_shared<Shader>("assets/shaders/model_loading_vs.glsl", "assets/shaders/model_loading_fs.glsl");
	m_DebugShader = std::make_shared<Shader>("assets/shaders/debug_vs.glsl", "assets/shaders/debug_fs.glsl");
	m_Skybox = std::make_shared<Skybox>(SkyboxData::faces);

	setupCamera();
	setupCube(shader);
	setupFloor(shader);
	setupSphere(shader);
}

void MainLevel::update(float deltaTime) {
	if (deltaTime > 0.1f) deltaTime = 0.1f;

	handlePhysics(deltaTime);
	
	for (auto& actor : m_Actors) actor->updateComponents(deltaTime);

	/*float time = static_cast<float>(glfwGetTime());

	for (size_t i = 0; i < m_CubeActors.size(); i++) {
		auto transformComponent = m_CubeActors[i]->getComponent<TransformComponent>();
		if (!transformComponent) continue;

		if (i % 2 == 0) {
			glm::vec3 currentRotation = transformComponent->getRotation();
			currentRotation.y += deltaTime * 50.0f;
			transformComponent->setRotation(currentRotation);
		}
		else {
			glm::vec3 position = transformComponent->getPosition();
			position.x += cos(time + i) * 0.02f;
			position.y += sin(time * 2.0f + i) * 0.01f;
			transformComponent->setPosition(position);
		}
	}

	for (auto& actor : m_Actors) actor->updateComponents(deltaTime);*/
}

void MainLevel::render() {
	for (auto& actor : m_Actors) {
		actor->renderComponents();
	}

	renderDebugCollision();
}

void MainLevel::setupCamera() {
	/*auto cameraActor = spawnActor();
	cameraActor->addComponent<TransformComponent>()->setPosition({ 0.0f, 1.0f, 5.0f });
	m_MainCamera = cameraActor->addComponent<CameraComponent>();*/

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

void MainLevel::setupCube(std::shared_ptr<Shader> shader) {
	auto cubeModel = std::make_shared<Model>("assets/models/cube.fbx");
	auto cubeMaterial = std::make_shared<Material>(shader);
	Texture cubeTexture = TextureLoader::loadTexture("assets/images/plank/Planks012_2K-PNG_Color.png");
	cubeMaterial->setTexture(cubeTexture.id);

	for (int i = 0; i < 5; i++) {
		auto actor = spawnActor();
		auto transformComponent = actor->addComponent<TransformComponent>();
		float randX = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
		float randZ = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
		float height = 10.0f + (i * 2.0f);
		transformComponent->setPosition({ randX, height, randZ });

		actor->addComponent<MeshRendererComponent>(cubeModel, cubeMaterial);

		auto rb = actor->addComponent<RigidBodyComponent>();
		rb->setMass(1.0f);
		rb->setBounciness(0.6f);
		rb->init();

		rb->addTorque(glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX) * 5.0f);
		//rb->applyImpulse(glm::vec3((float)i * 0.1f, 0, 0));

		m_PhysicsActors.push_back(actor);
	}

	/*m_CubeActor = spawnActor();
	auto transformComponent = m_CubeActor->addComponent<TransformComponent>();
	transformComponent->setPosition({ 0.0f, 5.0f, 0.0f });
	transformComponent->setScale({ 1.0f, 1.0f, 1.0f });

	m_CubeActor->addComponent<MeshRendererComponent>(cubeModel, cubeMaterial);
	m_CubeVelocity = glm::vec3(0.0f);*/

	/*for (int i = 0; i < 10; i++) {
		auto actor = spawnActor();
		auto transformComponent = actor->addComponent<TransformComponent>();

		float x = static_cast<float>((rand() % 20) - 10);
		float y = static_cast<float>((rand() % 5) + 5);
		float z = static_cast<float>((rand() % 20) - 10);

		transformComponent->setPosition({ x, y, z });
		actor->addComponent<MeshRendererComponent>(cubeModel, cubeMaterial);
		m_CubeActors.push_back(actor);
	}*/
}

void MainLevel::setupFloor(std::shared_ptr<Shader> shader) {
	auto floorMaterial = std::make_shared<Material>(shader);
	Texture floorTexture = TextureLoader::loadTexture("assets/textures/grid/128x128/Gray/Prototype_Grid_Gray_07-128x128.png");
	floorMaterial->setTexture(floorTexture.id);

	m_FloorActor = spawnActor();
	auto floorTransform = m_FloorActor->addComponent<TransformComponent>();
	floorTransform->setPosition({ 0.0f, 0.0f, 0.0f });
	floorTransform->setScale({ 1.0f, 1.0f, 1.0f });

	/*auto floorModel = std::make_shared<Model>("assets/models/floor.fbx");
	m_FloorActor->addComponent<MeshRendererComponent>(floorModel, floorMaterial);*/

	auto floorPrimitive = m_FloorActor->addComponent<PrimitiveComponent>(PrimitiveType::E_Plane, floorMaterial);
	floorPrimitive->init();
}

void MainLevel::setupSphere(std::shared_ptr<Shader> shader) {
	auto material = std::make_shared<Material>(shader);
	for (int i = 0; i < 5; i++) {
		auto actor = spawnActor(); 
		auto transform = actor->addComponent<TransformComponent>();

		float radius = 0.5f;
		float rx = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
		float rz = ((float)rand() / RAND_MAX) * 4.0f - 2.0f;
		transform->setPosition({ rx, 15.0f + (i * 2.0f), rz });
		transform->setScale(glm::vec3(radius * 2.0f));

		actor->addComponent<PrimitiveComponent>(PrimitiveType::E_Sphere, material)->init();

		auto rb = actor->addComponent<RigidBodyComponent>();
		rb->setMass(0.8f);
		rb->setBounciness(0.8f);

		auto col = std::make_unique<SphereCollider>();
		col->radius = radius;
		rb->setCollider(std::move(col));

		rb->init();
		m_PhysicsActors.push_back(actor);
	}

	/*m_SphereActor = spawnActor();
	m_SphereActor->addComponent<TransformComponent>()->setPosition({ 2.0f, 5.0f, 0.0f });
	m_SphereActor->addComponent<PrimitiveComponent>(PrimitiveType::E_Sphere, material)->init();*/
}

void MainLevel::handlePhysics(float deltaTime) {
	auto floorTransform = m_FloorActor->getComponent<TransformComponent>();
	auto floorBox = Collision::getAABB(floorTransform->getPosition() - glm::vec3(0, 0.5f, 0), glm::vec3(40.0f, 1.0f, 40.0f));

	for (size_t i = 0; i < m_PhysicsActors.size(); i++) {
		auto actor = m_PhysicsActors[i];
		auto rbActor = actor->getComponent<RigidBodyComponent>();
		auto transformActor = actor->getComponent<TransformComponent>();

		// 1. Gravity
		rbActor->addForce(glm::vec3(0.0f, m_GRAVITY * rbActor->getMass(), 0.0f));

		// 2. Floor Collision
		auto boxA = Collision::getAABB(transformActor->getPosition(), transformActor->getScale());
		auto manifoldFloor = Collision::getCollisionManifold(boxA, floorBox);
		if (manifoldFloor.isColliding) {
			Collision::Resolve(rbActor, nullptr, manifoldFloor);
			transformActor->setPosition(transformActor->getPosition() + manifoldFloor.normal * manifoldFloor.penetrationDepth);
		}

		// 3. Object vs Object Collision (The "Double Loop")
		for (size_t j = i + 1; j < m_PhysicsActors.size(); j++) {
			auto b = m_PhysicsActors[j];
			auto rbB = b->getComponent<RigidBodyComponent>();
			auto transB = b->getComponent<TransformComponent>();

			auto boxB = Collision::getAABB(transB->getPosition(), transB->getScale());
			auto manifoldOBB = Collision::getCollisionManifold(boxA, boxB);

			if (manifoldOBB.isColliding) {
				Collision::Resolve(rbActor, rbB, manifoldOBB);

				// Jitter Fix: push them apart so they don't stick
				float totalInvMass = (1.0f / rbActor->getMass()) + (1.0f / rbB->getMass());
				glm::vec3 correction = (manifoldOBB.penetrationDepth / totalInvMass) * 0.5f * manifoldOBB.normal;
				transformActor->setPosition(transformActor->getPosition() + correction);
				transB->setPosition(transB->getPosition() - correction);
			}
		}
	}

	//if (!cubeTransform || !floorTransform) return;

	//m_CubeVelocity.y += m_GRAVITY * deltaTime;
	//glm::vec3 position = cubeTransform->getPosition();
	//position += m_CubeVelocity * deltaTime;
	//cubeTransform->setPosition(position);

	//auto floorBox = Collision::getAABB(floorTransform->getPosition() - glm::vec3(0, 1.0f, 0), glm::vec3(20.0f, 2.0f, 20.0f));
	//auto cubeBox = Collision::getAABB(cubeTransform->getPosition(), cubeTransform->getScale() * 1.0f);

	//if (Collision::checkAABB(cubeBox, floorBox)) {
	//	position.y = floorBox.max.y + (cubeTransform->getScale().y * 0.5f);
	//	cubeTransform->setPosition(position);
	//	m_CubeVelocity.y = 0.0f;
	//}

	//// Advanced Check: Reset if falling
	//if (position.y < -10.0f) {
	//	cubeTransform->setPosition({ 0.0f, 5.0f, 0.0f });
	//	m_CubeVelocity = glm::vec3(0.0f);
	//	std::cout << "[PHYSICS] Cube reset - fell through floor!" << std::endl;
	//}

	//// Sphere Physics
	//if (m_SphereActor) {
	//	auto transform = m_SphereActor->getComponent<TransformComponent>();
	//	glm::vec3 position = transform->getPosition();
	//	glm::vec3 rotation = transform->getRotation();
	//	float radius = transform->getScale().y * 0.5f;

	//	// Gravity
	//	m_SphereVelocity.y += m_GRAVITY * deltaTime;
	//	position += m_SphereVelocity * deltaTime;

	//	// Collision with floor
	//	float floorLevel = 0.0f;
	//	if (position.y - radius < floorLevel) {
	//		position.y = floorLevel + radius;
	//		m_SphereVelocity.y = -m_SphereVelocity.y * 0.5f;
	//		m_SphereVelocity.x *= 0.99f;
	//	}

	//	if(glm::abs(m_SphereVelocity.x) > 0.001f) {
	//		float distanceMoved = m_SphereVelocity.x * deltaTime;
	//		float angleChange = (distanceMoved / radius) * (180.0f / 3.14159f);
	//		rotation.z -= angleChange;
	//	}

	//	transform->setPosition(position);
	//	transform->setRotation(rotation);
	//}
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

	//auto floorTransform = m_FloorActor->getComponent<TransformComponent>();
	//auto floorBox = Collision::getAABB(floorTransform->getPosition() - glm::vec3(0, 1.0f, 0), glm::vec3(20.0f, 2.0f, 20.0f));
	//Collision::drawAABB(floorBox, glm::vec3(0, 1, 0), m_DebugShader);

	/*for (auto* actor : m_PhysicsActors) {
		auto transformComponent = actor->getComponent<TransformComponent>();
		if (transformComponent) {
			auto box = Collision::getAABB(transformComponent->getPosition(), transformComponent->getScale());
			Collision::drawAABB(box, glm::vec3(1, 0, 0), m_DebugShader);
		}
	}*/

	for (auto* actor : m_PhysicsActors) {
		auto rb = actor->getComponent<RigidBodyComponent>();
		auto trans = actor->getComponent<TransformComponent>();
		if (!rb || !trans || !rb->getCollider()) continue;

		auto col = rb->getCollider();
		if (col->getType() == ColliderType::E_Sphere) {
			float r = static_cast<SphereCollider*>(col)->radius;
			Collision::drawSphere(trans->getPosition(), r, glm::vec3(0, 0, 1), m_DebugShader); // BLUE
		}
		else {
			glm::vec3 h = static_cast<BoxCollider*>(col)->halfSize;
			Collision::drawAABB({ trans->getPosition() - h, trans->getPosition() + h }, glm::vec3(1, 0, 0), m_DebugShader); // RED
		}
	}

	//auto floorBox = Collision::getAABB(floorTransform->getPosition() - glm::vec3(0, 1.0f, 0), glm::vec3(20.0f, 2.0f, 20.0f));
	//auto cubeBox = Collision::getAABB(cubeTransform->getPosition(), cubeTransform->getScale());
	//Collision::drawAABB(floorBox, glm::vec3(0, 1, 0), m_DebugShader); // Green
	//Collision::drawAABB(cubeBox, glm::vec3(1, 0, 0), m_DebugShader);
}

//void MainLevel::setupSculpture() {
//	auto sculptureMaterial = std::make_shared<Material>(shader);
//	Texture sculptureTexture = TextureLoader::loadTexture("assets/models/laurel_grove_cemetrey_angel/LaurelGroveCemetary101_diffuse.jpg");
//	floorMaterial->setTexture(sculptureTexture.id);
//
//	m_SculptureActor = spawnActor();
//	auto sculptureTransform = m_SculptureActor->addComponent<TransformComponent>();
//	sculptureTransform->setPosition({ 0.0f, 0.7f, 0.0f });
//	sculptureTransform->setScale({ 0.5f, 0.5f, 0.5f });
//
//	auto SculptureModel = std::make_shared<Model>("assets/models/laurel_grove_cemetrey_angel/LaurelGroveCemetary1.obj");
//	m_SculptureActor->addComponent<MeshRendererComponent>(SculptureModel, sculptureMaterial);
//
//}
