#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include "App/data/Texture.h"

class Model;
class Mesh;

class TextureLoader {
public:
	static void applyTexture(Model& model, const std::string& texturePath, const std::string& type = "texture_diffuse");
	static void applyTexture(Mesh& mesh, const std::string& texturePath, const std::string& type = "texture_diffuse");
	static Texture loadTexture(const std::string& path, const std::string& type = "texture_diffuse");
};

#endif // !TEXTURELOADER_H