#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 color;

out vec3 Normal;
out vec2 TexCoords;
out vec3 Color;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
	Normal = (transpose(inverse(model)) * vec4(normal, 1.0)).xyz;
	TexCoords = texCoords;
	Color = color;
	FragPos = (model * vec4(position, 1.0f)).xyz;
}