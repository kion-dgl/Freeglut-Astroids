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

struct PLAYER_STRUCT {
	
	GLfloat x;
	GLfloat y;
	GLfloat dx;
	GLfloat dy;
	GLfloat rot;
	GLfloat assetMatrix[16];

} Player;


int main( int argc, char *argv[] ) {

	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 480);
	glutCreateWindow("Main Window");
	glutFullScreen();

	Player.x = 400.0;
	Player.y = 100.0;
	Player.dx = 6.0;
	Player.dy = 6.0;
	Player.rot = 0.0;

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
	
	int i;
	for(i = 0; i < 16; i++){
		printf("%f\n", matrixOrtho2d[i]);
	}

	glUniformMatrix4fv(uniform_matrixOrtho2d, 1, GL_FALSE, matrixOrtho2d);

	return 0;

}


void on_display() {

	glClear(GL_COLOR_BUFFER_BIT);
		
	if(GAMEPAD_LEFT){
		Player.x -= Player.dx;
	}

	if(GAMEPAD_RIGHT){
		Player.x += Player.dx;
	}

	if(GAMEPAD_UP){
		Player.y += Player.dy;
	}

	if(GAMEPAD_DOWN) {
		Player.y -= Player.dy;
	}

	if(GAMEPAD_BUTTON_L) {
		Player.rot += 4.0;
	}

	if(GAMEPAD_BUTTON_R) {
		Player.rot -= 4.0;
	}

	mtxSetIdentity(Player.assetMatrix);
	mtxTranslateMatrix(Player.assetMatrix, Player.x, Player.y, 0.0);
	mtxRotateZMatrix(Player.assetMatrix, Player.rot);
	mtxScaleMatrix(Player.assetMatrix, 2.5, 2.5, 2.5);

	glUniformMatrix4fv(uniform_matrixModel, 1, GL_FALSE, Player.assetMatrix);

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

