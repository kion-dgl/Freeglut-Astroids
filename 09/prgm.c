#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "libs/mtx_utils.h"

int init_resources();
int free_resources();

void on_display();
void on_timer(int value);
void on_joystick(unsigned int button_mask, int x, int y, int z);

GLuint vbo_triangle;
GLuint program;
GLint attribute_coord2d;
GLint uniform_matrixOrtho2d;
GLint uniform_matrixModel;

#define VIEWPORT_WIDTH 800
#define VIEWPORT_HEIGHT 480

#define BUTTON_A 0x01
#define BUTTON_B 0x02
#define BUTTON_X 0x04
#define BUTTON_Y 0x08
#define BUTTON_L 0x10
#define BUTTON_R 0x20
#define BUTTON_SELECT 0x40
#define BUTTON_START  0x80

struct PLAYER_STRUCT {
	
	GLfloat x;
	GLfloat y;
	GLfloat dx;
	GLfloat dy;
	GLfloat rot;

	bool right;
	bool left;
	bool down;
	bool up;

	bool button_a;
	bool button_b;
	bool button_x;
	bool button_y;
	bool button_l;
	bool button_r;

	bool btn_start;
	bool btn_select;
	
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
	glutJoystickFunc(on_joystick, 25);
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

int free_resources(){
	
	return 0;

}

void on_display() {

	glClear(GL_COLOR_BUFFER_BIT);
		
	if(Player.left){
		Player.x -= Player.dx;
	}

	if(Player.right){
		Player.x += Player.dx;
	}

	if(Player.up){
		Player.y += Player.dy;
	}

	if(Player.down) {
		Player.y -= Player.dy;
	}

	if(Player.button_l) {
		Player.rot += 4.0;
	}

	if(Player.button_r) {
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

void on_joystick(unsigned int button_mask, int x, int y, int z){
	
	Player.left = (x < 0) ? true : false;
	Player.right = (x > 0) ? true : false;
	Player.up = (y < 0) ? true : false;
	Player.down = (y > 0) ? true : false;

	Player.button_a = button_mask & BUTTON_A ? true : false;
	Player.button_b = button_mask & BUTTON_B ? true : false;
	Player.button_x = button_mask & BUTTON_X ? true : false;
	Player.button_y = button_mask & BUTTON_Y ? true : false;
	Player.button_l = button_mask & BUTTON_L ? true : false;
	Player.button_r = button_mask & BUTTON_R ? true : false;

	Player.btn_start = button_mask & BUTTON_START ? true : false;
	Player.btn_select = button_mask & BUTTON_SELECT ? true : false;

}
