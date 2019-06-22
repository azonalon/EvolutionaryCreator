#version 450 core

layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};
// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 vertex;

// Output data ; will be interpolated for each fragment.
out vec2 UV;


void main(){

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  view*vec4(vertex, 0, 1);
    // UV of the vertex. No special space for this one.
    if(gl_VertexID == 0) {
        UV = vec2(0, 0);
    } else if(gl_VertexID == 3) {
        UV = vec2(0, 1);
    } else if(gl_VertexID == 2) {
        UV = vec2(1, 1);
    } else if(gl_VertexID == 1) {
        UV = vec2(1, 0);
    }
}