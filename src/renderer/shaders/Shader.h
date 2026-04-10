#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Shader {
public:
	~Shader();
	unsigned int id;

	Shader(const std::string& vertexSrc, const std::string& fragmentSrc, bool isRawString);
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	virtual void activate();

	// uniform functions
	void setBool(const std::string& name, bool value);
	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);

	void setVec2(const std::string& name, const glm::vec2& value);
	void setVec2(const std::string& name, float x, float y);

	void setVec3(const std::string& name, const glm::vec3& value);
	void setVec3(const std::string& name, float x, float y, float z);

	void setVec4(const std::string& name, const glm::vec4& value);
	void setVec4(const std::string& name, float x, float y, float z, float w);

	void setMat2(const std::string& name, const glm::mat2& mat);
	void setMat3(const std::string& name, const glm::mat3& mat);
	void setMat4(const std::string& name, const glm::mat4& mat);

protected:
	// utility functions
	virtual std::string loadShaderSrc(const char* filePath);
	virtual GLuint compileShader(const char* source, GLenum type);
	static unsigned int m_currentProgram;
	std::unordered_map<std::string, int > uniformCache;
	virtual GLint getUniformLocation(const std::string& name);
	void createProgram(const char* vertexCode, const char* fragmentCode);
};
#endif // !SHADER_H
