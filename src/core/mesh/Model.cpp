#include "pch.h"
#include "Model.h"
#include "core/mesh/Mesh.h"
#include <stb/stb_image.h>
#include "App/Data/Texture.h"

Model::Model(const std::string& path, bool gamma) {
	loadModel(path);
}

Model::~Model() {
	m_Meshes.clear();
}

void Model::draw(Shader& shader) {
	for (auto& mesh : m_Meshes)
		mesh.draw(shader);
}

void Model::loadModel(const std::string& path) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_Directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::addMesh(std::unique_ptr<Mesh> mesh) {
	m_Meshes.push_back(std::move(*mesh));
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;

		// Positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Normals
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		// Texture coordinates
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string t_typename) {
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string fullPath = m_Directory + "/" + std::string(str.C_Str());
		bool skip = false;
		for (Texture& t : m_Textures_Loaded) {
			if (strcmp(t.path.data(), str.C_Str()) == 0) {
				textures.push_back(t);
				skip = true;
				break;
			}
		}

		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), m_Directory);
			texture.type = t_typename;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_Textures_Loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma) {
	std::string filename = directory + '/' + std::string(path);
	std::cout << "Loading texture: " << filename << std::endl;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	if (!data) {
		std::cout << "Failed to load texture: " << path << " Reason: " << stbi_failure_reason() << std::endl;
		glDeleteTextures(1, &textureID);
		return 0;
	}

	GLenum format = GL_RGB;
	if (channels == 1)      format = GL_RED;
	else if (channels == 3) format = GL_RGB;
	else if (channels == 4) format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(data);

	return textureID;
}
