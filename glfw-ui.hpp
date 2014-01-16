#ifndef GLFW_UI_H_
#define GLFW_UI_H_

#include <GLFW/glfw3.h>


GLFWwindow * init_gl(GLFWwindow * window);

void error_callback(int error, const char* description);

GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
#endif