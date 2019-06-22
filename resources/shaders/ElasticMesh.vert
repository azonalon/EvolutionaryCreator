#version 450 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 texCoord;
layout(location=2) in float stretchFactor;
layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};
out VertexData {
    vec2 texCoord;
    vec4 vertexColor;
} VertexOut;

void main() {
  VertexOut.texCoord = texCoord;
  VertexOut.vertexColor = vec4(stretchFactor, 0, 1- stretchFactor,1);
  gl_Position = view*vec4(position, 0.0, 1.0);
}
