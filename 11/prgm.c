#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "libs/mtx_utils.h"
#include "libs/gamepad_utils.h"

int init_resources();
int free_resources();

void on_display();
void on_timer(int value);

GLuint vbo_triangle;
GLuint program;
GLint attribute_coord2d;
GLint uniform_matrixOrtho2d;
GLint uniform_matrixModel;

#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 480

struct mtxObject player;

int main( int argc, char *argv[] ) {

	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 480);
	glutCreateWindow("Main Window");
	glutFullScreen();
	
	
	player.pos[0] = 400.0;
	player.pos[1] = 100.0;
	player.pos[2] = 0.0;
	
	player.rot[0] = 0.0;
	player.rot[1] = 0.0;
	player.rot[2] = 0.0;
	
	player.scl[0] = 2.0;
	player.scl[1] = 2.0;
	player.scl[2] = 2.0;

	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if(!GLEW_VERSION_2_0) {
		fprintf(stderr, "Your gpu does not support OpenGL 2.0\n");
		return 1;
	}

	if(init_resources() < 0){
		fprintf(stderr, "Could not initialize resources\n");
		return 1;
	}

	glutDisplayFunc(on_display);
	glutTimerFunc(0, on_timer, 0);
	glutJoystickFunc(gamepad_callback, 25);
	glutMainLoop();

	free_resources();
	return 0;

}

int init_resources( ) {
	
	GLfloat triangle_vertices[] = {
		0.0, 10.0, -10.0, -10.0, 10.0, -10.0
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	program = mtxCreateProgram("shdr/vertex.glsl", "shdr/fragment.glsl");	

	attribute_coord2d = mtxGetShaderAttribute(program, "coord2d");
	uniform_matrixOrtho2d = mtxGetShaderUniform(program, "matrixOrtho2d");
	uniform_matrixModel = mtxGetShaderUniform(program, "matrixModel");

	glUseProgram(program);
	GLfloat *matrixOrtho2d = (GLfloat*)malloc(16 * sizeof(GLfloat));
	
	mtxSetIdentity(matrixOrtho2d);
	mtxCreateOrtho2d(matrixOrtho2d, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
	glUniformMatrix4fv(uniform_matrixOrtho2d, 1, GL_FALSE, matrixOrtho2d);

	return 0;

}


void on_display() {

	glClear(GL_COLOR_BUFFER_BIT);
		
	if(GAMEPAD_LEFT){
		player.pos[0] -= 6.0;
	}

	if(GAMEPAD_RIGHT){
		player.pos[0] += 6.0;
	}

	if(GAMEPAD_UP){
		player.pos[1] += 6.0;
	}

	if(GAMEPAD_DOWN) {
		player.pos[1] -= 6.0;
	}

	if(GAMEPAD_BUTTON_L) {
		player.rot[2] += 4.0;
	}

	if(GAMEPAD_BUTTON_R) {
		player.rot[2] -= 4.0;
	}
	
	
	mtxTransformObject(&player);
	glUniformMatrix4fv(uniform_matrixModel, 1, GL_FALSE, player.matrix);
	

	glEnableVertexAttribArray(attribute_coord2d);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_coord2d,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(attribute_coord2d);

	glutSwapBuffers();

}

void on_timer(int value) {
	
	glutPostRedisplay();
	glutTimerFunc(30, on_timer, 0);

}

int free_resources(){
	
	return 0;

}

