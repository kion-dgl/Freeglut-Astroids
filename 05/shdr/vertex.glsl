attribute vec2 coord2d;
uniform mat4 matrixOrtho2d;

void main(void) {

	gl_Position = matrixOrtho2d * vec4(coord2d, 0.0, 1.0);

}
