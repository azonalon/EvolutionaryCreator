#version 450
layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;
// uniform mat4 viewMatrix; // GeeXLab auto uniform
// uniform vec2 WIN_SCALE;

out vec3 dist;

  void main() {
    vec4 p0_3d = gl_in[0].gl_Position;
    vec4 p1_3d = gl_in[1].gl_Position;
    vec4 p2_3d = gl_in[2].gl_Position;
    gl_Position = p0_3d;
    EmitVertex();
    gl_Position = p1_3d;
    EmitVertex();
    gl_Position = p2_3d;
    EmitVertex();

    //--------------------------------
    EndPrimitive();
  }
