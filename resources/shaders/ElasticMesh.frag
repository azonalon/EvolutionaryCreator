#version 450
// uniform vec3 WIRE_COL;
// uniform vec3 FILL_COL;
in vec3 dist;
out vec4 FragColor;
uniform sampler2D Tex;
in VertexData {
    vec2 texCoord;
    vec4 vertexColor;
} VertexIn;
void main()
{
  // Undo perspective correction.
  //vec3 dist_vec = dist * gl_FragCoord.w;

  // Wireframe rendering is better like this:
  vec3 dist_vec = dist;

  // Compute the shortest distance to the edge
  float d = min(dist_vec[0], min(dist_vec[1], dist_vec[2]));

  // Compute line intensity and then fragment color
  float I = exp2(-100.0*d);
  // float I = 1;

  vec4 texColor = texture(Tex, VertexIn.texCoord);
  vec3 WIRE_COL = vec3(0.1, 0.1, 0.1);
  // vec3 FILL_COL = vec3(0.5, 0.5, 0.8);
  vec3 FILL_COL = texColor.rgb;
  // vec3 FILL_COL = VertexIn.vertexColor.rgb;
  FragColor.rgb = I*WIRE_COL + (1.0 - I)*FILL_COL;
  FragColor.a = texColor.a;
  FragColor.a = 1;
}
