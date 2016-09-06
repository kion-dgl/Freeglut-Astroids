attribute vec2 coord2d;
uniform mat4 matrixOrtho2d;
uniform mat4 matrixModel;

void main(void) {
	
	vec4 modelPos = matrixModel * vec4(coord2d, 0.0, 1.0);
	gl_Position = matrixOrtho2d * modelPos;

}
