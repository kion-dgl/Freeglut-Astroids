#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../lib/kinectGL.h"

GLuint vbo_triangle;
GLuint program;
GLint attribute_coord2d;
GLint uniform_matrixOrtho2d;
GLint uniform_matrixModel;
GLint uniform_matrixRotation;

int init_resources();
void onDisplay();
void free_resources();

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitContextVersion(2, 0);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(800, 480);
	glutCreateWindow("My second triangle");
	glutFullScreen();

	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if(!GLEW_VERSION_2_0) {
		fprintf(stderr, "Your gpu does not support OpenGL 2.0\n");
		return 1;
	}

	if(init_resources()) {
		glutDisplayFunc(onDisplay);
		glutMainLoop();
	}

	free_resources();
	return 0;

}

int init_resources() {

	GLfloat triangle_vertices[] = {
		0.0, 20.0, -20.0, -20.0, 20.0, -20.0
	};

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	program = kiCreateProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
	
	const char *attribute_name = "coord2d";
	attribute_coord2d = glGetAttribLocation(program, attribute_name);
	if(attribute_coord2d == -1) {
		fprintf(stderr, "Could not bind attribute: %s\n", attribute_name);
		return 0;
	}

	const char *uniform_name = "matrixOrtho2d";
	uniform_matrixOrtho2d = glGetUniformLocation(program, uniform_name);
	if(uniform_matrixOrtho2d == -1) {
		fprintf(stderr, "Could not bind uniform: %s\n", uniform_name);
		return 0;
	}
	
	uniform_name = "matrixModel";
	uniform_matrixModel = glGetUniformLocation(program, uniform_name);
	if(uniform_matrixModel == -1) {
		fprintf(stderr, "Could not bind uniform: %s\n", uniform_name);
		return 0;
	}

	uniform_name = "matrixRotation";
	uniform_matrixRotation = glGetUniformLocation(program, uniform_name);
	if(uniform_matrixRotation == -1) {
		fprintf(stderr, "Could not bind uniform: %s\n", uniform_name);
		return 0;
	}

	return 1;

}

void onDisplay() {

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
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

	GLfloat rot[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	kiRotateZ(rot, 45.0);
	glUniformMatrix4fv(uniform_matrixRotation, 1, GL_FALSE, rot);

	GLfloat matrix[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	kiOrtho2D(matrix, 0.0, 800.0, 0.0, 480.0);	
	glUniformMatrix4fv(uniform_matrixOrtho2d, 1, GL_FALSE, matrix);

	GLfloat model[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	kiTranslate(model, 100.0, 100.0, 0.0);
	
	glUniformMatrix4fv(uniform_matrixModel, 1, GL_FALSE, model);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(attribute_coord2d);
	glutSwapBuffers();

}

void free_resources() {

	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);

}
