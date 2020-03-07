#version 450
layout(std140, binding = 2) uniform SharedUniforms { mat4 view; };
in vec2 texCoord;
in vec4 vertexPosition;
out vec4 fragColor;
// in vec4 fragCoordinate;
vec4 u_major_grid_color = vec4(0.8, 0.8, 0.8, 1);
vec4 u_minor_grid_color = vec4(0.8, 0.8, 0.8, 0.5);
vec2 u_iResolution = vec2(10, 10);
float u_antialias = 1.0;
const float M_PI = 3.14159265358979323846;



#ifdef POLAR
vec4 u_limits1 = vec4(-0.01,+0.01,-0.01,+0.01);
vec4 u_limits2 = vec4(-5.0,+5.0, M_PI/6.0, 11.0*M_PI/6.0);
vec2 u_major_grid_step = vec2(1.0, M_PI/6.0);
vec2 u_minor_grid_step = vec2(0.25, M_PI/60.0);
float u_minor_grid_width = 0.0075;
float u_major_grid_width = 0.0150;

vec2 transform_forward(vec2 P)
{
  float x = P.x * cos(P.y);
  float y = P.x * sin(P.y);
  return vec2(x,y);
}

vec2 transform_inverse(vec2 P)
{
  float rho = length(P);
  float theta = atan(P.y,P.x);
  if( theta < 0.0 )
  theta = 2.0*M_PI+theta;
  return vec2(rho,theta);
}
#endif

//
vec2 u_major_grid_step = vec2(0.1, 0.1);
vec2 u_minor_grid_step = vec2(0.01, 0.01);
float u_minor_grid_width = 0.0075;
float u_major_grid_width = 2.0 * u_minor_grid_width;
vec4 u_limits1 = vec4(-0.01, +0.01, -0.01, +0.01);
vec4 u_limits2 = vec4(-1000, 1000, -1000, +1000);
// Forward transform (polar)
vec2 transform_forward(vec2 P) {
  return P;
}
// Inverse transform (polar)
vec2 transform_inverse(vec2 P) {
  return P;
}

// [-0.5,-0.5]x[0.5,0.5] -> [xmin,xmax]x[ymin,ymax]
vec2 scale_forward(vec2 P, vec4 limits) {
  // limits = xmin,xmax,ymin,ymax
  P += vec2(0.5, 0.5);
  P *= vec2(limits[1] - limits[0], limits[3] - limits[2]);
  P += vec2(limits[0], limits[2]);
  return P;
}
// [xmin,xmax]x[ymin,ymax] -> [-0.5,-0.5]x[0.5,0.5]
vec2 scale_inverse(vec2 P, vec4 limits) {
  // limits = xmin,xmax,ymin,ymax
  P -= vec2(limits[0], limits[2]);
  P /= vec2(limits[1] - limits[0], limits[3] - limits[2]);
  return P - vec2(.5, .5);
}
  // Antialias stroke alpha coeff
float stroke_alpha(float distance, float linewidth, float antialias) {
  float t = linewidth / 2.0 - antialias;
  float signed_distance = distance;
  float border_distance = abs(signed_distance) - t;
  float alpha = border_distance / antialias;
  alpha = exp(-alpha * alpha);
  if (border_distance > (linewidth / 2.0 + antialias))
    return 0.0;
  else if (border_distance < 0.0)
    return 1.0;
  else
    return alpha;
}
// Compute the nearest tick from a (normalized) t value
float get_tick(float t, float vmin, float vmax, float step) {
  float first_tick = floor((vmin + step / 2.0) / step) * step;
  float last_tick = floor((vmax + step / 2.0) / step) * step;
  float tick = vmin + t * (vmax - vmin);
  if (tick < (vmin + (first_tick - vmin) / 2.0)) return vmin;
  if (tick > (last_tick + (vmax - last_tick) / 2.0)) return vmax;
  tick += step / 2.0;
  tick = floor(tick / step) * step;
  return min(max(vmin, tick), vmax);
}
// Compute the distance (in screen coordinates) between A and B
float screen_distance(vec4 A, vec4 B) {
  vec4 pA = view * A;
  pA /= pA.w;
  pA.xy = pA.xy * u_iResolution / 2.0;
  vec4 pB = view * B;
  pB /= pB.w;
  pB.xy = pB.xy * u_iResolution / 2.0;
  return length(pA.xy - pB.xy);
}

void main() {
  vec2 NP1 = texCoord;
  vec2 P1 = scale_forward(NP1, u_limits1);
  vec2 P2 = transform_inverse(P1);
  // Test if we are within limits but we do not discard the
  // fragment yet because we want to draw border. Discarding
  // would mean that the exterior would not be drawn.
  bvec2 outside = bvec2(false);
  if (P2.x < u_limits2[0]) outside.x = true;
  if (P2.x > u_limits2[1]) outside.x = true;
  if (P2.y < u_limits2[2]) outside.y = true;
  if (P2.y > u_limits2[3]) outside.y = true;
  vec2 NP2 = scale_inverse(P2, u_limits2);
  vec2 P;
  float tick;
  // Major tick, X axis
  tick = get_tick(NP2.x + .5, u_limits2[0], u_limits2[1], u_major_grid_step[0]);
  P = transform_forward(vec2(tick, P2.y));
  P = scale_inverse(P, u_limits1);
  // float Mx = length(v_size * (NP1 - P));
  // Here we assume the quad is contained in the XZ plane
  float Mx = screen_distance(vec4(NP1, 0, 1), vec4(P, 0, 1));
  // Minor tick, X axis
  tick = get_tick(NP2.x + .5, u_limits2[0], u_limits2[1], u_minor_grid_step[0]);
  P = transform_forward(vec2(tick, P2.y));
  P = scale_inverse(P, u_limits1);
  // float mx = length(v_size * (NP1 - P));
  // Here we assume the quad is contained in the XZ plane
  float mx = screen_distance(vec4(NP1, 0, 1), vec4(P, 0, 1));
  // Major tick, Y axis
  tick = get_tick(NP2.y + .5, u_limits2[2], u_limits2[3], u_major_grid_step[1]);
  P = transform_forward(vec2(P2.x, tick));
  P = scale_inverse(P, u_limits1);
  // float My = length(v_size * (NP1 - P));
  // Here we assume the quad is contained in the XZ plane
  float My = screen_distance(vec4(NP1, 0, 1), vec4(P, 0, 1));
  // Minor tick, Y axis
  tick = get_tick(NP2.y + .5, u_limits2[2], u_limits2[3], u_minor_grid_step[1]);
  P = transform_forward(vec2(P2.x, tick));
  P = scale_inverse(P, u_limits1);
  // float my = length(v_size * (NP1 - P));
  // Here we assume the quad is contained in the XZ planefloat my =
  // screen_distance(vec4(NP1,0,1), vec4(P,0,1));
  float my = screen_distance(vec4(NP1,0,1), vec4(P,0,1));

  float M = min(Mx, My);
  float m = min(mx, my);
  // Here we take care of "finishing" the border lines
  if (outside.x && outside.y) {
    if (Mx > 0.5 * (u_major_grid_width + u_antialias)) {
      discard;
    } else if (My > 0.5 * (u_major_grid_width + u_antialias)) {
      discard;
    } else {
      M = max(Mx, My);
    }
  } else if (outside.x) {
    if (Mx > 0.5 * (u_major_grid_width + u_antialias)) {
      discard;
    } else {
      M = m = Mx;
    }
  } else if (outside.y) {
    if (My > 0.5 * (u_major_grid_width + u_antialias)) {
      discard;
    } else {
      M = m = My;
    }
  }
  // Mix major/minor colors to get dominant color
  vec4 color = u_major_grid_color;
  float alpha1 = stroke_alpha(M, u_major_grid_width, u_antialias);
  float alpha2 = stroke_alpha(m, u_minor_grid_width, u_antialias);
  float alpha = alpha1;
  if (alpha2 > alpha1 * 1.5) {
    alpha = alpha2;
    color = u_minor_grid_color;
  }
  // Without extra cost, we can also project a texture
  // vec4 texcolor = texture2D(u_texture, vec2(NP2.x, 1.0-NP2.y));
  // gl_FragColor = mix(texcolor, color, alpha);
  fragColor = vec4(color.rgb, color.a * alpha);
}