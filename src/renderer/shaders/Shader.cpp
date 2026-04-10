#include "pch.h"
#include "Shader.h"

unsigned int Shader::m_currentProgram = 0;

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc, bool isRawString) {
	createProgram(vertexSrc.c_str(), fragmentSrc.c_str());
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//std::string vertexCode = loadShaderSrc(vertexShaderPath);
	//std::string fragmentCode = loadShaderSrc(fragmentShaderPath);

	//const char* vShaderCode = vertexCode.c_str();
	//const char* fShaderCode = fragmentCode.c_str();

	//GLuint vertex = compileShader(vShaderCode, GL_VERTEX_SHADER);
	//GLuint fragment = compileShader(fShaderCode, GL_FRAGMENT_SHADER);

	//id = glCreateProgram();
	//glAttachShader(id, vertex);
	//glAttachShader(id, fragment);
	//glLinkProgram(id);

	//int success;
	//char infoLog[512];

	//glGetProgramiv(id, GL_LINK_STATUS, &success);
	//if (!success) {
	//	glGetProgramInfoLog(id, 512, NULL, infoLog);
	//	std::cout << "Shader linking error\n" << infoLog << std::endl;
	//}
	//glDeleteShader(vertex);
	//glDeleteShader(fragment);

	std::string vertexCode = loadShaderSrc(vertexShaderPath);
	std::string fragmentCode = loadShaderSrc(fragmentShaderPath);

	createProgram(vertexCode.c_str(), fragmentCode.c_str());
}

Shader::~Shader() {
	glDeleteProgram(id);
}

void Shader::activate() {
	if (m_currentProgram != id) {
		glUseProgram(id);
		m_currentProgram = id;
	}
}

std::string Shader::loadShaderSrc(const char* filePath) {
	std::ifstream file;
	std::stringstream buffer;

	file.open(filePath);
	if (!file.is_open()) {
		//throw std::runtime_error("Failed to open shader file");
		std::string msg = "Failed to open shader file at: ";
		msg += filePath;
		throw std::runtime_error(msg);
	}

	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

GLuint Shader::compileShader(const char* source, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Shader compile error\n" << infoLog << std::endl;
	}
	return shader;
}

GLint Shader::getUniformLocation(const std::string& name) {
	if (uniformCache.count(name))
		return uniformCache.at(name);

	int location = glGetUniformLocation(id, name.c_str());
	uniformCache[name] = location;
	return location;
}

void Shader::createProgram(const char* vertexCode, const char* fragmentCode) {
	GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
	GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	glLinkProgram(id);

	// Error Checking
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader linking error\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::setBool(const std::string& name, bool value) {
	glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
	glUniform2fv(getUniformLocation(name), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) {
	glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
	glUniform3f(getUniformLocation(name), x, y, z);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) {
	glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) {
	glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) {
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) {
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}