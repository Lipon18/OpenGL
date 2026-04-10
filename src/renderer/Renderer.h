#ifndef RENDERER_H
#define RENDERER_H

class Skybox;
class CameraComponent;

class Renderer {
public:
	static bool showDepth;

	static void setShowDepth(bool value) { showDepth = value; }
	static bool getShowDepth() { return showDepth; }
	static void DrawSkybox(std::shared_ptr<Skybox>& skybox, CameraComponent* camera, float aspect);
};

#endif // !RENDERER_H