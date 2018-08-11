#version 450 core

layout(location=0) in vec2 position;
layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};


void main() {
  gl_Position = view*vec4(position, 0.0, 1);
}
