#ifndef MODEL_H
#define MODEL_H

//#include "core/mesh/Mesh.h"

class Shader;
class Mesh;
struct Texture;

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {
public:
    Model(const std::string& path, bool gamma = false);
    ~Model();
    void draw(Shader& shader);
    void addMesh(std::unique_ptr<Mesh> mesh);

    const std::vector<Mesh>& getMesh() const { return m_Meshes; }
    std::vector<Mesh>& getMesh() { return m_Meshes; }
private:
    std::vector<Mesh> m_Meshes;
    std::vector<Texture> m_Textures_Loaded;
    std::string m_Directory;
    bool m_GammaCorrection;

    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string t_typename);
};

#endif