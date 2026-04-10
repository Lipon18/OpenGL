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
#include  "core/components/PrimitiveComponent.h"
#include "renderer/skybox/SkyboxData.h"
#include "renderer/skybox/Skybox.h"

MainLevel::MainLevel() {}
MainLevel::~MainLevel() {}

void MainLevel::init() {
	auto lineShader = std::make_shared<Shader>("assets/shaders/line_vs.glsl", "assets/shaders/line_fs.glsl");
	auto shader = std::make_shared<Shader>("assets/shaders/model_loading_vs.glsl", "assets/shaders/model_loading_fs.glsl");
	m_Skybox = std::make_shared<Skybox>(SkyboxData::faces);

	setupCamera();
	setupCube(shader);
	setupFloor(shader);
}

void MainLevel::update(float deltaTime) {
	for (auto& actor : m_Actors) actor->updateComponents(deltaTime);
}

void MainLevel::render() {
	for (auto& actor : m_Actors) {
		actor->renderComponents();
	}
}

void MainLevel::setupCamera() {
	auto cameraActor = spawnActor();
	cameraActor->addComponent<TransformComponent>()->setPosition({ 0.0f, 1.0f, 5.0f });
	m_MainCamera = cameraActor->addComponent<CameraComponent>();
}

void MainLevel::setupCube(std::shared_ptr<Shader> shader) {
	auto cubeMaterial = std::make_shared<Material>(shader);
	Texture cubeTexture = TextureLoader::loadTexture("assets/images/plank/Planks012_2K-PNG_Color.png");
	cubeMaterial->setTexture(cubeTexture.id);

	m_CubeActor = spawnActor();
	auto cubeTransform = m_CubeActor->addComponent<TransformComponent>();
	cubeTransform->setPosition({ 0.0f, 1.1f, 0.0f });
	cubeTransform->setScale({ 1.0f, 1.0f, 1.0f });

	auto cubeModel = std::make_shared<Model>("assets/models/cube.fbx");
	m_CubeActor->addComponent<MeshRendererComponent>(cubeModel, cubeMaterial);
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
