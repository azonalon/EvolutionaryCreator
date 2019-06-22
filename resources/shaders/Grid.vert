#version 450 core

layout(location=0) in vec2 position;
layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};

out vec2 gridCoordinate;
out vec2 lineWidths;
out float ratio;

void main() {
    float scaleX = 2/view[0][0];
    float scaleY = 2/view[1][1];
    ratio = (1+floor(10/scaleX));
    gridCoordinate = vec2(scaleX*position.x, scaleX*position.y);
    gl_Position = view*vec4(gridCoordinate, 0.0, 1);
    lineWidths = vec2(0.001*scaleX, 0.001*scaleX);
}