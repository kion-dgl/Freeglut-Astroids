attribute vec2 coord2d;
uniform mat4 matrixOrtho2d;
uniform mat4 matrixModel;
uniform mat4 matrixRotation;

void main(void) {
	
	vec4 rotPos = matrixRotation * vec4(coord2d, 0.0, 1.0);
	vec4 modelPos = matrixModel * rotPos;
	gl_Position = matrixOrtho2d * modelPos;

}
