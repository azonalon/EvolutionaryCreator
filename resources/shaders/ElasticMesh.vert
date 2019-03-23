#version 450 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoord;
layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};
out VertexData {
    vec2 texCoord;
} VertexOut;

void main() {
  VertexOut.texCoord = texCoord;
  gl_Position = view*vec4(position, 0.0, 1.0);
}
