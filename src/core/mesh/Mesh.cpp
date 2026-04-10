#include "pch.h"
#include "Mesh.h"
#include "renderer/shaders/Shader.h"
#include <stb/stb_image.h>
#include "Model.h"
//#include "App/data/Texture.h"

//Mesh::Mesh(float* vertices, size_t vSize, const std::string& texturePath, unsigned int* indices, size_t iSize) {
//	// =======================
//	// OBJECT VAO AND LIGHT VAO
//	// =======================
//	glGenVertexArrays(1, &cubeVAO);
//	glGenVertexArrays(1, &lightVAO);
//	glGenBuffers(1, &VBO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);
//
//	// position attribute
//	glBindVertexArray(cubeVAO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	// normal attribute
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//	// texture
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//
//	// Setup Light VAO (Uses the same VBO data)
//	glBindVertexArray(lightVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glBindVertexArray(0);
//
//	// Vertex count (always compute)
//	vertexCount = static_cast<GLsizei>(vSize / (3 * sizeof(float)));
//
//	// Check if EBO is provided
//	if (indices != nullptr && iSize > 0) {
//		hasEBO = true;
//		indexCount = iSize / sizeof(unsigned int);
//
//		glGenBuffers(1, &EBO);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
//	}
//	else {
//		hasEBO = false;
//		indexCount = 0;
//		//vertexCount = vSize / (3 * sizeof(float)); /**/
//	}
//	texture = loadTexture(texturePath);
//}
//
////Mesh::Mesh(float* vertices, size_t vSize, const std::string& texturePath) {
////	hasEBO = false;
////	vertexCount = vSize / (5 * sizeof(float));
////
////	glGenVertexArrays(1, &VAO);
////	glGenBuffers(1, &VBO);
////
////	glBindVertexArray(VAO);
////
////	// VBO
////	glBindBuffer(GL_ARRAY_BUFFER, VBO);
////	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);
////
////	// Vertex attributes
////	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
////	glEnableVertexAttribArray(0);
////	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
////	glEnableVertexAttribArray(1);
////
////	texture = loadTexture(texturePath);
////
////	glBindVertexArray(0);
////}
//
//Mesh::~Mesh() {
//	glDeleteVertexArrays(1, &cubeVAO);
//	glDeleteVertexArrays(1, &lightVAO);
//	glDeleteBuffers(1, &VBO);
//	if (hasEBO) glDeleteBuffers(1, &EBO);
//	glDeleteTextures(1, &texture);
//}
//
//void Mesh::draw() {
//	glBindTexture(GL_TEXTURE_2D, texture);
//	glBindVertexArray(cubeVAO);
//	if (hasEBO) glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
//	else glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
//}
//
//void Mesh::drawLight() {
//	glBindVertexArray(lightVAO);
//	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
//}
//
//unsigned int Mesh::loadTexture(const std::string& path) {
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, channels;
//	stbi_set_flip_vertically_on_load(true);
//
//	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
//
//	if (!data) {
//		std::cout << "Failed to load texture: " << path << std::endl;
//		return 0;
//	}
//
//	GLenum format = 0;
//	if (channels == 1) format = GL_RED;
//	else if (channels == 3) format = GL_RGB;
//	else if (channels == 4) format = GL_RGBA;
//
//	glBindTexture(GL_TEXTURE_2D, textureID);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	// Wrapping
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	// Filtering
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	stbi_image_free(data);
//
//	return textureID;
//}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures) : 
	m_Vertices(vertices), m_Indices(indices), m_Textures(textures) {

	setupMesh();
}

void Mesh::draw(Shader& shader) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = m_Textures[i].type;
		if (name == "texture_diffuse")       number = std::to_string(diffuseNr++);
		else if (name == "texture_specular") number = std::to_string(specularNr++);
		else if (name == "texture_normal")   number = std::to_string(normalNr++);
		else if (name == "texture_height")   number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
	}
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// Tangents
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// Bitangents
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
	// Bone IDs
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
	// Weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

	glBindVertexArray(0);
}
