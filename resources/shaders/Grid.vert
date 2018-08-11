#version 450 core

layout(location=0) in vec2 position;
layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};

out vec2 gridCoordinate;

void main() {
    gridCoordinate = 10*position;
    gl_Position = view*vec4(10*position, 0.0, 1);
}
