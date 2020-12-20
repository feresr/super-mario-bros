#version 330 core

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tx;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 texture;


void main() {
    TexCoords = (texture * vec4(tx, 0.0, 1.0)).xy;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}