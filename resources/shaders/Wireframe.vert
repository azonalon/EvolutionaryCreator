#version 450 core

layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};

layout(location = 0) in vec2 vertex;

void main(){
    gl_Position =  view*vec4(vertex, 0, 1);
}