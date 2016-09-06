#include <string.h>
#include <GL/glew.h>
#include <math.h>

#define M_00 0x00
#define M_10 0x01
#define M_20 0x02
#define M_30 0x03

#define M_01 0x04
#define M_11 0x05
#define M_21 0x06
#define M_31 0x07

#define M_02 0x08
#define M_12 0x09
#define M_22 0x0a
#define M_32 0x0b

#define M_03 0x0c
#define M_13 0x0d
#define M_23 0x0e
#define M_33 0x0f

GLuint kiCreateShader(const char *filename, GLenum type) {

	// Read the shader file
	FILE *fp = fopen(filename, "rb");
	if(fp == NULL) {
		
		fprintf(stderr, "Could not open %s\n", filename);
		return 0;

	}

	// Get length of source file
	fseek(fp, 0, SEEK_END);
	int fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Read file as character pointer
	char *fileSource = (char*)malloc(fileLength + 1);
	fread(&fileSource[0], 1, fileLength, fp);
	fileSource[fileLength] = '\0';

	// Close file
	fclose(fp);

	// Append Shader header
	const GLchar *sources[] = {
	#ifdef GL_ES_VERSION_2_0
		"#version 100\n"
	#else
		"#version 120\n"
	#endif
		, fileSource
	};

	// Compile shader source
	GLuint compile_ok = GL_FALSE;
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 2, sources, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	free(fileSource);

	// Get log if compile error
	if(!compile_ok) {

		fprintf(stderr, "Error in shader: %s\n", filename);
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char *log = (char*)malloc(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, log);
		fprintf(stderr, "%s\n", log);
		free(log);
		return 0;

	}

	return shader;
	
}

GLuint kiCreateProgram(const char *vertexFilename, const char *fragmentFilename) {

	GLuint vs = kiCreateShader(vertexFilename, GL_VERTEX_SHADER);
	if(vs == 0){	
		return 0;
	}

	GLuint fs = kiCreateShader(fragmentFilename, GL_FRAGMENT_SHADER);
	if(fs == 0) {
		return 0;
	}

	GLint link_ok = GL_FALSE;
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	
	if(!link_ok) {
		fprintf(stderr, "glLinkProgram Error\n");
		return 0;
	}

	return program;

}

void kiOrtho2D(GLfloat* m, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top) {

	const GLfloat zNear = -.10f;
	const GLfloat zFar = 1.0f;
	const GLfloat inv_z = 1.0f / (zFar - zNear);
	const GLfloat inv_y = 1.0f / (top - bottom);
	const GLfloat inv_x = 1.0f / (right - left);
	
	//First column
	m[M_00] = 2.0f * inv_x;
	m[M_10] = 0.0f;
	m[M_20] = 0.0f;
	m[M_30] = 0.0f;

	//Second column
	m[M_01] = 0.0f;
	m[M_11] = 2.0f * inv_y;
	m[M_21] = 0.0f;
	m[M_31] = 0.0f;
	
	//Third column
	m[M_02] = 0.0f;
	m[M_12] = 0.0f;
	m[M_22] = -2.0f * inv_z;
	m[M_32] = 0.0f;
	
	//Fourth column
	m[M_03] = -(right + left)*inv_x;
	m[M_13] = -(top + bottom)*inv_y;
	m[M_23] = -(zFar + zNear)*inv_z;
	m[M_33] = 1.0f;

}

void kiMatrixMultiply(GLfloat *a, GLfloat *b) {
	
	GLfloat c[] = {
		1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1
	};
	
	// First Row
	c[M_00] = a[M_00]*b[M_00] + a[M_01]*b[M_10] + a[M_02]*b[M_20] + a[M_03]*b[M_30];
	c[M_01] = a[M_00]*b[M_01] + a[M_01]*b[M_11] + a[M_02]*b[M_21] + a[M_03]*b[M_31];
	c[M_02] = a[M_00]*b[M_02] + a[M_01]*b[M_12] + a[M_02]*b[M_22] + a[M_03]*b[M_32];
	c[M_03] = a[M_00]*b[M_03] + a[M_01]*b[M_13] + a[M_02]*b[M_23] + a[M_03]*b[M_33];

	// Second Row
	c[M_10] = a[M_10]*b[M_00] + a[M_11]*b[M_10] + a[M_12]*b[M_20] + a[M_13]*b[M_30];
	c[M_11] = a[M_10]*b[M_01] + a[M_11]*b[M_11] + a[M_12]*b[M_21] + a[M_13]*b[M_31];
	c[M_12] = a[M_10]*b[M_02] + a[M_11]*b[M_12] + a[M_12]*b[M_22] + a[M_13]*b[M_32];
	c[M_13] = a[M_10]*b[M_03] + a[M_11]*b[M_13] + a[M_12]*b[M_23] + a[M_13]*b[M_33];

	// Third Row
	c[M_20] = a[M_20]*b[M_00] + a[M_21]*b[M_10] + a[M_22]*b[M_20] + a[M_23]*b[M_30];
	c[M_21] = a[M_20]*b[M_01] + a[M_21]*b[M_11] + a[M_22]*b[M_21] + a[M_23]*b[M_31];
	c[M_22] = a[M_20]*b[M_02] + a[M_21]*b[M_12] + a[M_22]*b[M_22] + a[M_23]*b[M_32];
	c[M_23] = a[M_20]*b[M_03] + a[M_21]*b[M_13] + a[M_22]*b[M_23] + a[M_23]*b[M_33];

	// Fourth Row
	c[M_30] = a[M_30]*b[M_00] + a[M_31]*b[M_10] + a[M_32]*b[M_20] + a[M_33]*b[M_30];
	c[M_31] = a[M_30]*b[M_01] + a[M_31]*b[M_11] + a[M_32]*b[M_21] + a[M_33]*b[M_31];
	c[M_32] = a[M_30]*b[M_02] + a[M_31]*b[M_12] + a[M_32]*b[M_22] + a[M_33]*b[M_32];
	c[M_33] = a[M_30]*b[M_03] + a[M_31]*b[M_13] + a[M_32]*b[M_23] + a[M_33]*b[M_33];
	
	// Copy product to first element

	a[0] = c[0]; a[1] = c[1]; a[2] = c[2]; a[3] = c[3];
	a[4] = c[4]; a[5] = c[5]; a[6] = c[6]; a[7] = c[7];
	a[8] = c[8]; a[9] = c[9]; a[10] = c[10]; a[11] = c[11];
	a[12] = c[12]; a[13] = c[13]; a[14] = c[14]; a[15] = c[15];

}

void kiTranslate(GLfloat *m, GLfloat x, GLfloat y, GLfloat z) {

	GLfloat trans[] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	};
	
	kiMatrixMultiply(m, trans);

}

void kiRotateZ(GLfloat *m, GLfloat zAngle) {
	
	GLfloat radians = zAngle * M_PI / 180.0;
	
	GLfloat c = (GLfloat)cos(radians);
	GLfloat s = (GLfloat)sin(radians);

	GLfloat rotZ[] = {
		c,-s, 0, 0,
		s, c, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	
	kiMatrixMultiply(m, rotZ);

}
