#include <stdio.h>
#include <stdlib.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

GLFWwindow *win;

void handleFrambufferResize(GLFWwindow *win, int w, int h);
int glfw_init(void);
int glfw_create_window(void);
int glad_init(void);

void run(void);
void term(int status);

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
run(void)
{
	while(!glfwWindowShouldClose(win)) {
		glfwSwapBuffers(win);
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void
term(int status)
{
	glfwDestroyWindow(win);
	glfwTerminate();

	exit(status);
}

int
main()
{
	if (!glfw_init() || !glfw_create_window() || !glad_init())
		term(1);

	run();

	term(0);
}
