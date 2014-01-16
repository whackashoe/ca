#include <iostream>
#include <vector>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glfw-ui.hpp"

void error_callback(int error, const char* description)
{
	std::cerr << "glfw err:" << error << " :: " << description << std::endl;
}


GLFWwindow * init_gl(GLFWwindow * window)
{
	glfwSetErrorCallback(error_callback);
	if(!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 2 );
    glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	window = glfwCreateWindow(640, 480, "HaRDCASh", NULL, NULL);
	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if(glewError != GLEW_OK ) {
		std::cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << std::endl;
		exit(EXIT_FAILURE);
	}

	//Make sure OpenGL 2.1 is supported
	if(!GLEW_VERSION_2_1) {
		std::cerr << "OpenGL 2.1 not supported!" << std::endl;
		exit(EXIT_FAILURE);
	}

	return window;
}

GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open())
	{
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
 
	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}
 
	GLint Result = GL_FALSE;
	int InfoLogLength;
 
	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
 
	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> VertexShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	if(!VertexShaderErrorMessage.empty())
		std::cerr << vertex_file_path << " :: " << &VertexShaderErrorMessage[0] << std::endl;

	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);
 
	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	if(!FragmentShaderErrorMessage.empty())
		std::cerr << fragment_file_path << " :: " << &FragmentShaderErrorMessage[0] << std::endl;
 
	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
 
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(0)) );
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	if(!ProgramErrorMessage.empty())
		std::cerr << "glsl program :: " << &ProgramErrorMessage[0] << std::endl;
 
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
 
	return ProgramID;
}