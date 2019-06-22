#version 450
out vec4 color;
in vec2 gridCoordinate;
in vec2 lineWidths;
in float ratio;
void main() {
    vec2 p = fract(gridCoordinate*ratio)/ratio;
    if(p.x > lineWidths.x && p.y > lineWidths.y) {
        color = vec4(0.0,0.0,0.0,1.0);
        discard;
    }
    color = vec4(0.8,0.8,0.8,1.0);
}
