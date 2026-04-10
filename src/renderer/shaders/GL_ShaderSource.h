#ifndef GL_ShaderSource
#define GL_ShaderSource

namespace Shaders {
	// Vertex Shader
	const char* model_loading_vs = R"glsl(
	#version 330 core
	layout (location = 0) in vec3 aPos;
	layout (location = 1) in vec3 aNormal;
	layout (location = 2) in vec2 aTexCoords;

	out vec2 TexCoords;

	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main() {
		TexCoords = aTexCoords;
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
)glsl";

	// Fragment Shader
	const char* model_loading_fs = R"glsl(
	#version 330 core
	out vec4 FragColor;

	in vec2 TexCoords;

	uniform sampler2D texture_diffuse1;

	void main() {
		FragColor = texture(texture_diffuse1, TexCoords);
	}
)glsl";

}
#endif // !GL_ShaderSource