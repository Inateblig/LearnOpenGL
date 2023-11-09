#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "loadfile.h"
#include "draw.h"


GLFWwindow *win;

void handleFrambufferResize(GLFWwindow *win, int w, int h);
int glfw_init(void);
int glfw_create_window(void);
int glad_init(void);

void setup(void);
void run(void);
void term(int status);

void gettargetdir(char *dirbuf, char *argv0);

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f,
};
GLint vertexColorLocation;
GLuint prog, VBO, VAO;

void
handleFrambufferResize(GLFWwindow *win, int w, int h)
{
	glViewport(0, 0, w, h);
}

int
glfw_init(void)
{
	if(!glfwInit())
		return 0;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	return 1;
}

int
glfw_create_window(void)
{
	win = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (win == NULL) {
		printf("Failed to create GLFW win");
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(win);
	glfwSetFramebufferSizeCallback(win, &handleFrambufferResize);

	return 1;
}

int
glad_init(void)
{
	if (gladLoadGL(glfwGetProcAddress))
		return 1;

	printf("Failed to initialize GLAD");
	return 0;
}

void
setup(void)
{
	char *shd_src[2];
	GLuint shds[2];

	shd_src[0] = loadfile("data/shaders/vertex.shader");
	shd_src[1] = loadfile("data/shaders/fragment.shader");

	if ((!compile_shader(&shds[0], GL_VERTEX_SHADER, shd_src[0])) ||
		!compile_shader(&shds[1], GL_FRAGMENT_SHADER, shd_src[1])) {
		fprintf(stderr, "hello I am under the return 0\n");
		return;
	}

	if (!link_prog(&prog, sizeof (shds) / sizeof (shds)[0], shds)) {
		fprintf(stderr, "hello I am under the return 1\n");
		return;
	}

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	vertexColorLocation = glGetUniformLocation(prog, "ourColor");
}

void
run(void)
{
	GLdouble timeValue;
	GLfloat greenValue;
	while(!glfwWindowShouldClose(win)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		timeValue = glfwGetTime();
		greenValue = (sin(timeValue) / 2.0 + 0.5);

		glUseProgram(prog);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

void
term(int status)
{
	glfwDestroyWindow(win);
	glfwTerminate();

	exit(status);
}

void
gettargetdir(char *dirbuf, char *argv0) {
	size_t off;
	char *ls; /* last slash '/' */

	if (!(ls = strrchr(argv0, '/'))) {
		strcpy(dirbuf, ".");
		return;
	}

	off = ls - argv0;
	strncpy(dirbuf, argv0, off);
	dirbuf[off] = '\0';
}

int
main(int argc, char *argv[])
{
	char targetdir[PATH_MAX];

	gettargetdir(targetdir, argv[0]);
	chdir(targetdir);

	if (!glfw_init() || !glfw_create_window() || !glad_init())
		term(1);

	setup();
	run();

	term(0);
}
