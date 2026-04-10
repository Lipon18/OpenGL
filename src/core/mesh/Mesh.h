#ifndef MESH_H
#define MESH_H

#include "App/data/Texture.h"

#define MAX_BONE_INFLUENCE 4

class Shader;
//struct Texture;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE] = { 0 };
	float m_Weights[MAX_BONE_INFLUENCE] = { 0.0f };
};

class Mesh {
public:
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);
	void draw(Shader& shader);

	std::vector<Texture>& getMeshTexture() { return m_Textures; }
	const std::vector<Texture>& getMeshTexture() const { return m_Textures; }

	unsigned int VAO;
	//~Mesh();
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture> m_Textures;

	unsigned int VBO, EBO;
	void setupMesh();
	//unsigned int loadTexture(const std::string& path);
};

#endif // !MESH_H