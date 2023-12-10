#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define CGLM_DEFINE_PRINTS
#include <cglm/cglm.h>

#include "loadfile.h"
#include "draw.h"
#include "loadimage.h"


GLFWwindow *win;

void handleFrambufferResize(GLFWwindow *win, int w, int h);
void cursor_callback(GLFWwindow* win, double xpos, double ypos);
int glfw_init(void);
int glfw_create_window(void);
int glad_init(void);

void setup(void);
void run(void);
void process_input(GLFWwindow *win);
void term(int status);

void gettargetdir(char *dirbuf, char *argv0);

float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
vec3 cubePos[] = {
	{ 0.0f,  0.0f,  0.0f},
	{ 2.0f,  5.0f, -15.0f},
	{-1.5f, -2.2f, -2.5f},
	{-3.8f, -2.0f, -12.3f},
	{ 2.4f, -0.4f, -3.5f},
	{-1.7f,  3.0f, -7.5f},
	{ 1.3f, -2.0f, -2.5f},
	{ 1.5f,  2.0f, -2.5f},
	{ 1.5f,  0.2f, -1.5f},
	{-1.3f,  1.0f, -1.5f},
};
vec3 camPos = (vec3){0.0f, 0.0f, 3.0f};
vec3 camFront = (vec3){0.0f, 0.0f, -1.0f};
vec3 camUp = (vec3){0.0f, 1.0f, 0.0f};
float yaw = -GLM_PI_2, pitch = 0.f;
float lastX = 400.f, lastY = 300.f;
int firstMouse = 1;

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;

GLuint VBO, VAO;
GLuint prog, textures[2];

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
	glfwSetCursorPosCallback(win, cursor_callback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	if ((!loadimage(&textures[0], "data/textures/clown.png"))) {
		fprintf(stderr, "hello I am under the texture return 0\n");
		return;
	}
	if ((!loadimage(&textures[1], "data/textures/mrgh0s7.png"))) {
		fprintf(stderr, "hello I am under the texture return 1\n");
		return;
	}

	glUseProgram(prog);
	glUniform1i(glGetUniformLocation(prog, "texture0"), 0);
	glUniform1i(glGetUniformLocation(prog, "texture1"), 1);

	glEnable(GL_DEPTH_TEST);
}

void
run(void)
{
	while(!glfwWindowShouldClose(win)) {
		int i;

		process_input(win);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glUseProgram(prog);

//		mat4 view = GLM_MAT4_IDENTITY_INIT;
		mat4 view;

		float time_f = (float) glfwGetTime();
		deltaTime = time_f - lastFrameTime;
		lastFrameTime = time_f;
		vec3 center;
		glm_vec3_add(camFront, camPos, center);
		glm_lookat(camPos, center, camUp, view);
//		glm_translate(view, (vec3){0.f, 0.f, -3.f});

		mat4 projection;
		glm_perspective(GLM_PI_4, 800.f / 600.f, 0.1f, 100.f, projection);

		glUniformMatrix4fv(glGetUniformLocation(prog, "view"), 1, GL_FALSE, view[0]);
		glUniformMatrix4fv(glGetUniformLocation(prog, "projection"), 1, GL_FALSE, projection[0]);

		glBindVertexArray(VAO);
		for (i = 0; i < 10; i++) {
			mat4 model = GLM_MAT4_IDENTITY_INIT;
			float angle = GLM_PI_4 / 2.f * i;
			glm_translate(model, cubePos[i]);
			glm_rotate(model, angle, (vec3){1.0f, 0.3f, 0.5f});
			glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, GL_FALSE, model[0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

void cursor_callback(GLFWwindow* win, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}
  
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos;

	float sens = 0.001f;
	xoffset *= sens;
	yoffset *= sens;

	yaw   += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	vec3 dir;
	dir[0] = cos(yaw) * cos(pitch);
	dir[1] = sin(pitch);
	dir[2] = sin(yaw) * cos(pitch);
	glm_vec3_normalize(dir);
	glm_vec3_copy(dir, camFront);
}

void
process_input(GLFWwindow *win)
{
	float camSpeed = 2.5f * deltaTime;
	vec3 camPos_prev;

	glm_vec3_copy(camPos, camPos_prev);
	if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) {
		glm_vec3_scale(camFront, camSpeed, camPos);
		glm_vec3_add(camPos, camPos_prev, camPos);
		glm_vec3_copy(camPos, camPos_prev);
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
		glm_vec3_scale(camFront, -camSpeed, camPos);
		glm_vec3_add(camPos, camPos_prev, camPos);
		glm_vec3_copy(camPos, camPos_prev);
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		glm_vec3_crossn(camFront, camUp, camPos);
		glm_vec3_scale(camPos, -camSpeed, camPos);
		glm_vec3_add(camPos, camPos_prev, camPos);
		glm_vec3_copy(camPos, camPos_prev);
	}
	if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
		glm_vec3_crossn(camFront, camUp, camPos);
		glm_vec3_scale(camPos, camSpeed, camPos);
		glm_vec3_add(camPos, camPos_prev, camPos);
		glm_vec3_copy(camPos, camPos_prev);
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
