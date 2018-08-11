#version 450
out vec4 color;
in vec2 gridCoordinate;
void main() {
    vec2 p = fract(gridCoordinate);
    if(p.x > 0.01 && p.y > 0.01) {
        color = vec4(0.0,0.0,0.0,1.0);
        discard;
    }
    color = vec4(0.8,0.8,0.8,1.0);
}
