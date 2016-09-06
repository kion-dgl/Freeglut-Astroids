#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

#define PI 3.141592
#define WIDTH 640
#define HEIGHT 480
#define TITLE "Orthagonal Matrix"

GLfloat ballX = 100.0f; 
GLfloat ballY = 100.0f;
GLfloat ballXSpeed = 5.0f;
GLfloat ballYSpeed = 5.0f;
GLfloat ballRadius = 14.0f;

bool left_key_down = false;
bool right_key_down = false;

GLfloat paddleDx = 6.0f;
GLfloat paddleX = 320.0f;
GLfloat paddleY = 20.0f;
GLfloat paddleHeight = 20.0f;
GLfloat paddleWidth = 80.0f;

GLfloat ballYMin;
GLfloat ballYMax;
GLfloat ballXMin;
GLfloat ballXMax;

void initGL();
void display();
void timer(int value);
void keydown_callback(int key, int x, int y);
void keyup_callback(int key, int x, int y);
void joystick(unsigned int buttonmask, int x, int y, int z);

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow(TITLE);
	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glutSpecialFunc(keydown_callback);
	glutSpecialUpFunc(keyup_callback);
	glutJoystickFunc(joystick, 100);
	initGL();
	glutMainLoop();
}

void initGL(){
	ballYMin = ballRadius;
	ballYMax = HEIGHT - ballRadius;
	ballXMin = ballRadius;
	ballXMax = WIDTH - ballRadius;
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,WIDTH,0,HEIGHT);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(ballX, ballY, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	GLfloat angle;
	
	int i;
	for(i = 0; i <= 100; i++){
		angle = i * 2.0f * PI / 100;
		glVertex2f(cos(angle)*ballRadius, sin(angle)*ballRadius);
	}
	glEnd();
	glPopMatrix();

	if(left_key_down){
		paddleX -= paddleDx;
	}else if(right_key_down){
		paddleX += paddleDx;
	}

	glPushMatrix();
	glTranslatef(paddleX, paddleY, 0.0f);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(-paddleWidth, 0.0f);
	glVertex2f(paddleWidth, 0.0f);
	glVertex2f(paddleWidth, paddleHeight);
	glVertex2f(-paddleWidth, paddleHeight);
	glEnd();
	glPopMatrix();
	
	glutSwapBuffers();
	
	ballX += ballXSpeed;
	ballY += ballYSpeed;

	if(ballX > ballXMax){
		ballX = ballXMax;
		ballXSpeed = -ballXSpeed;
	}else if(ballX < ballXMin){
		ballX = ballXMin;
		ballXSpeed = -ballXSpeed;
	}

	if(ballY > ballYMax){
		ballY = ballYMax;
		ballYSpeed = -ballYSpeed;
	}else if(ballY < ballYMin){
		ballY = ballYMin;
		ballYSpeed = -ballYSpeed;
	}
}

void timer(int value){
	glutPostRedisplay();
	glutTimerFunc(30, timer, 0);
}

void keydown_callback(int key, int x, int y){
	switch(key){
		case GLUT_KEY_RIGHT:
			right_key_down = true;			
		break;
		case GLUT_KEY_LEFT:
			left_key_down = true;
		break;
	}
}

void keyup_callback(int key, int x, int y){
	switch(key){
		case GLUT_KEY_RIGHT:
			right_key_down = false;			
		break;
		case GLUT_KEY_LEFT:
			left_key_down = false;
		break;
	}
}


void joystick(unsigned int buttonmask, int x, int y, int z){

	printf("X: %d, Y: %d, Z: %d\n", x, y, z);
	printf("Button mask %d\n", buttonmask);

}
