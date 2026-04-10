#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
uniform vec2 uvScale;
uniform vec2 uvOffset;
uniform bool useColor;
uniform bool showDepth;

float near = 0.1f; 
float far  = 100.0f;

float LinearizeDepth(float depth) {
    float z = depth * 2.0f - 1.0f;
    return (2.0f * near * far) / (far + near - z * (far - near));
}

void main() {
    if(showDepth) {
        float depth = LinearizeDepth(gl_FragCoord.z) / far;
        FragColor = vec4(vec3(depth), 1.0f);
        return;
    }

    if(useColor)
        FragColor = vec4(objectColor, 1.0f);
    else {
        vec2 uv = TexCoords * uvScale + uvOffset;
        FragColor = texture(texture_diffuse1, uv);
        //FragColor = tex;
    }
}