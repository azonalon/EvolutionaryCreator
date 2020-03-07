#version 450 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;
layout(std140, binding = 2) uniform SharedUniforms { mat4 view; };
out vec2 texCoord;
out vec4 vertexPosition;

void main() {
  float scaleX = 2 / view[0][0];
  float scaleY = 2 / view[1][1];
  texCoord = uv;
  vertexPosition = view * vec4(position, 0, 1);
  gl_Position = vertexPosition;
}
