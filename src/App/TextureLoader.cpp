#include "pch.h"
#include "App/TextureLoader.h"
#include "core/mesh/Mesh.h"
#include "core/mesh/Model.h"
#include "renderer/shaders/Shader.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma); // existing function

void TextureLoader::applyTexture(Model& model, const std::string& texturePath, const std::string& type) {
    Texture tex = loadTexture(texturePath, type);

    // Apply to every mesh in the model
    for (auto& mesh : model.getMesh()) {
        mesh.getMeshTexture().clear(); // remove FBX default textures
        mesh.getMeshTexture().push_back(tex);
    }
}

void TextureLoader::applyTexture(Mesh& mesh, const std::string& texturePath, const std::string& type) {
    Texture tex = loadTexture(texturePath, type);

    if (tex.id == 0) {
        std::cout << "Warning: texture failed to load: " << texturePath << std::endl;
    }

    mesh.getMeshTexture().clear();
    mesh.getMeshTexture().push_back(tex);
}

Texture TextureLoader::loadTexture(const std::string& path, const std::string& type) {
    Texture tex;
    tex.id = TextureFromFile(path.c_str(), ".");
    tex.type = type;
    tex.path = path;
    return tex;
}
