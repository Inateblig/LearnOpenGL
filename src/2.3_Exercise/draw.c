#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>
#include "draw.h"

int
compile_shader(GLuint *sh, GLenum sht, const char *src)
{
	GLuint retSh;
	GLint st;

	if (!(retSh = glCreateShader(sht))) {
		fprintf(stderr, "glCreateShader() failed\n");
		return 0;
	}
	glShaderSource(retSh, 1, &src, NULL);
	glCompileShader(retSh);

	glGetShaderiv(retSh, GL_COMPILE_STATUS, &st);
	if (!st) {
		GLint loglen;
		GLchar *loginfo;
		const char *sht_nm;

		switch (sht) {
		case GL_VERTEX_SHADER:
			sht_nm = "Vertex";
			break;
		case GL_GEOMETRY_SHADER:
			sht_nm = "Geometry";
			break;
		case GL_FRAGMENT_SHADER:
			sht_nm = "Fragment";
			break;
		default:
			sht_nm = "(other type)";
			break;
		}

		glGetShaderiv(retSh, GL_INFO_LOG_LENGTH, &loglen);
		if (!(loginfo = malloc(loglen + 1))) {
			perror("malloc");
			return 0;
		}

		glGetShaderInfoLog(retSh, loglen, NULL, loginfo);
		loginfo[loglen] = '\0';
		fprintf(stderr, "%s shader's complimantion failed:\n%s",
			sht_nm, loginfo);

		free(loginfo);
		return 0;
	}

	*sh = retSh;
	return 1;
}

int
link_prog(GLuint *prog, size_t n, GLuint shds[n])
{
	GLint st;
	GLuint retprog;
	unsigned int i;

	if (!(retprog = glCreateProgram())) {
		fprintf(stderr, "glCreateProgram() failed\n");
		return 0;
	}

	for (i = 0; i < n; i++) {
		glAttachShader(retprog, shds[i]);
	}
	glLinkProgram(retprog);

	glGetProgramiv(retprog, GL_LINK_STATUS, &st);
	if (!st) {
		GLint loglen;
		GLchar *loginfo;

		glGetProgramiv(retprog, GL_INFO_LOG_LENGTH, &loglen);
		if (!(loginfo = malloc(loglen + 1))) {
			perror("malloc");
			return 0;
		}

		glGetProgramInfoLog(retprog, loglen, NULL, loginfo);
		loginfo[loglen] = '\0';
		fprintf(stderr, "Shader link failed:\n%s",
			loginfo);

		free(loginfo);
		return 0;
	}

	/* delete way 2 */
	for (i = 0; i < n; i++)
		glDeleteShader(shds[i]);

	*prog = retprog;
	return 1;
}
