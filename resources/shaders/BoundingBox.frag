#version 450
// uniform vec3 WIRE_COL;
// uniform vec3 FILL_COL;
in vec3 dist;
out vec4 FragColor;
void main()
{
  vec4 WIRE_COL = vec4(1, 1, 1, 1);
  vec4 FILL_COL = vec4(0, 0, 0, 1);
  // Undo perspective correction.
  //vec3 dist_vec = dist * gl_FragCoord.w;

  // Wireframe rendering is better like this:
  vec3 dist_vec = dist;

  // Compute the shortest distance to the edge
  float d = min(dist_vec[0], min(dist_vec[1], dist_vec[2]));

  // Compute line intensity and then fragment color
  float I;
  if(d < 0.005) {
      I=1;
  } else {
      I=0;
  }

  FragColor = I*WIRE_COL + (1.0 - I)*FILL_COL;
}
