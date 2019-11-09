#version 450 core

layout(location=0) in vec2 position;
layout(location=1) in vec2 uv;
layout (std140, binding=2) uniform SharedUniforms
{
    mat4 view;
};

const float SQRT_2 = 1.4142135623730951;
out float size, linewidth, antialias;
out vec4 fg_color, bg_color;
out vec2 rotation;
out float v_size;

void main(void) {
  size =20*view[0][0] * (10+log(length(uv)));
  linewidth = size/10;
  antialias = 1;
  fg_color = vec4(1,1,1,1);
  bg_color = vec4(.9,.9,.9,1);
  gl_Position = view * vec4(position, 0, 1.0);
  v_size = SQRT_2 * size + 2.0 * (linewidth + 1.5 * antialias);
  gl_PointSize = v_size;
  rotation = normalize(uv);
}