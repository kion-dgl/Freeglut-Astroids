/*
    
	Matrix Utilities
   
	Copyright (C) 2016 Benjamin Collins
    
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License version 3
    as published by the Free Software Foundation.
    
	This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.
    
	You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <math.h>
#include <stdlib.h>
#include <GL/glew.h>

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

GLuint mtxCreateShader(const char *filename, GLenum type);
GLuint mtxCreateProgram(const char *vs, const char *fs);
GLint mtxGetShaderUniform(GLuint program, const char *attribute_name);
GLint mtxGetShaderAttribute(GLuint program, const char *attribute_name);
GLfloat* mtxCreateIdentity();
GLfloat* mtxCreateOrtho2d(GLint width, GLint height);

/*
 * mtxCreate Shader
 */

GLuint mtxCreateShader(const char *filename, GLenum type) {

	FILE *fp;
	int file_len;
	char *shader_src;
	GLuint compile_ok, shader;

	fp = fopen(filename, "rb");
	
	if(fp == NULL){
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	file_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	shader_src = (char*)malloc(file_len + 1);
	fread(shader_src, 1, file_len, fp);
	shader_src[file_len] = '\0';
	
	fclose(fp);

	const GLchar *sources[] = {
	#ifdef GL_ES_VERSION_2_0
		"#version 100\n",
	#else
		"#version 120\n",
	#endif
		shader_src
	};
	
	compile_ok = GL_FALSE;
	shader = glCreateShader(type);
	glShaderSource(shader, 2, sources, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_ok);
	
	free(shader_src);
	
	if(compile_ok == GL_FALSE) {
		fprintf(stderr, "Error in shader: %s\n", filename);
		exit(1);
	}

	return shader;

}


/*
 * mtxCreate Shader
 */


GLuint mtxCreateProgram(const char *vs, const char *fs) {

	GLuint v_shader = mtxCreateShader(vs, GL_VERTEX_SHADER);
	if(v_shader == 0){
		fprintf(stderr, "Vertex shader creation error\n");
		exit(1);
	}

	GLuint f_shader = mtxCreateShader(fs, GL_FRAGMENT_SHADER);
	if(f_shader == 0){
		fprintf(stderr, "Fragment shader creation error\n");
		exit(1);
	}

	GLuint link_ok = GL_FALSE;
	GLuint program = glCreateProgram();
	glAttachShader(program, v_shader);
	glAttachShader(program, f_shader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

	if(link_ok == GL_FALSE){
		fprintf(stderr, "glLinkProgram Error\n");
		exit(1);
	}

	return program;

}

/*
 * mtx get shader uniform
 */

GLint mtxGetShaderUniform(GLuint program, const char *attribute_name) {

	GLint uniform_location = glGetUniformLocation(program, attribute_name);
	
	if(uniform_location == -1){
		fprintf(stderr, "Could not locate %s in shader.\n");
		exit(1);
	}

	return uniform_location;

}

/*
 * mtx get shader attribute
 */


GLint mtxGetShaderAttribute(GLuint program, const char *attribute_name) {

	GLint attrib_location = glGetAttribLocation(program, attribute_name);
	
	if(attrib_location == -1){
		fprintf(stderr, "Could not locate %s in shader.\n");
		exit(1);
	}

	return attrib_location;

}

GLfloat* mtxCreateIdentity() {
	
	GLfloat *mtx = malloc(16*sizeof(GLfloat));

	mtx[0] = 1.0;
	mtx[1] = 0.0;
	mtx[2] = 0.0;
	mtx[3] = 0.0;

	mtx[4] = 0.0;
	mtx[5] = 1.0;
	mtx[6] = 0.0;
	mtx[7] = 0.0;

	mtx[8] = 0.0;
	mtx[9] = 0.0;
	mtx[10] = 1.0;
	mtx[11] = 0.0;

	mtx[12] = 0.0;
	mtx[13] = 0.0;
	mtx[14] = 0.0;
	mtx[15] = 1.0;
	
	return mtx;

}

GLfloat* mtxCreateOrtho2d(GLint width, GLint height) {

	if(width == 0){
		fprintf(stderr, "mtxCreateOrtho2d width cannot be zero\n");
		exit(1);
	}

	if(height == 0){
		fprintf(stderr, "mtxCreateOrtho2d height cannot be zero\n");
		exit(1);
	}

	GLfloat right = (GLfloat)width;
	GLfloat top = (GLfloat)height;
	
	const GLfloat left = 0.0;
	const GLfloat bottom = 0.0;
	const GLfloat zNear = -.10f;
	const GLfloat zFar = 1.0f;
	const GLfloat inv_z = 1.0f / (zFar - zNear);
	const GLfloat inv_y = 1.0f / (top - bottom);
	const GLfloat inv_x = 1.0f / (right - left);

	GLfloat *m = (GLfloat*)malloc(16 * sizeof(GLfloat));

	m[M_00] = 2.0f * inv_x;
	m[M_10] = 0.0f;
	m[M_20] = 0.0f;
	m[M_30] = 0.0f;

	m[M_01] = 0.0f;
	m[M_11] = 2.0f * inv_y;
	m[M_21] = 0.0f;
	m[M_31] = 0.0f;

	m[M_02] = 0.0f;
	m[M_12] = 0.0f;
	m[M_22] = -2.0f * inv_z;
	m[M_32] = 0.0f;

	m[M_03] = -(right + left)*inv_x;
	m[M_13] = -(top + bottom)*inv_y;
	m[M_23] = -(zFar + zNear)*inv_z;
	m[M_33] = 1.0f;

	return m;

}
