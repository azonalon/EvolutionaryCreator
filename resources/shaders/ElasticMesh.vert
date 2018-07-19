#version 450

uniform mat4 viewMatrix;

layout(location=0) in vec2 position;

void main() {
  gl_Position = viewMatrix*vec4(position, 0.0, 1.0);
}
