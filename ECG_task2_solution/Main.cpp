/*
* Copyright 2020 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
* Joaquin Telleria 01408189
*/


#include "Utils.h"
#include <GLFW/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>

/* Prototypes */
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* Global variables */


/* Main */
int main(int argc, char** argv)
{
	/* Load settings.ini */
	INIReader reader("assets/settings.ini"); // init reader for ini files
	int width = reader.GetInteger("window", "width", 800); // load values from ini file
	int height = reader.GetInteger("window", "height", 800);
	std::string window_title = reader.Get("window", "title", "ECG 2020");

	/* Initialize scene */
	if (!glfwInit()) { // initialize GLFW
		std::cerr << "ERROR: GLFW Not Initialized"; // if GLFW is not initialized then deliver Error message... 
		return 0; //...and Exit program
	}

#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // specify OpenGL version, 3 "Major" and "minor" are two components of a single version number, separated by a dot.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version 4.3 is specified.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GLFW_OPENGL_PROFILE and GLFW_OPENGL_CORE_PROFILE specify which OpenGL profile to create the context for.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // specify a fixed size window

	GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);//Create Window

	if (!window) {
		glfwTerminate(); //Free all resources before exit
		std::cerr << "ERROR: Window could not be created"; // if Window failed to create then deliver Error message... 
		return 0; //...and Exit program
	}

	glfwMakeContextCurrent(window); //make sure that context window is active

	glewExperimental = true;  // To force GLEW to load all functions, the variable glewExperimental has to be modified:
	GLenum err = glewInit(); //initialize glew
	if (err != GLEW_OK) {
		std::cerr << "ERROR: GLEW failed to initialize"; // if GLEW failed to initialize then deliver Error message... 
		return 0; //...and Exit program
	}

    /* Init ECG framework */

    if (!initFramework()) {
        EXIT_WITH_ERROR("Failed to init framework");
    }

	glClearColor(1, 1, 1, 1); // set white as background color
	glViewport(0, 0, width, height); // set viewport transform

    /* Bind KeyButtons */
    glfwSetKeyCallback(window, key_callback);

    /* Compile Vertex Shader */
    const char* vertexSource;
    GLuint vertexShader;

    std::ifstream is_vs("C:/Users/Joaco/Desktop/Computer Graphics UE/Task_0/ECG_Solution/src/teapotRenderer.vert"); // read shader file
    const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>());
    vertexSource = f_vs.c_str();
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create an empty vertex shader handle
    glShaderSource(vertexShader, 1, &vertexSource, 0);
    glCompileShader(vertexShader); // Compile the vertex shader

    // Check for vs errors
    GLint succeded_vs;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succeded_vs);
    if (succeded_vs == GL_FALSE) {
        GLint logSize;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
        GLchar* message = new char[logSize];
        glGetShaderInfoLog(vertexShader, logSize, NULL, message);
        std::cerr << message;
        delete[] message;
    }

    /* Compile Fragment Shader */
    const char* fragmentSource;
    GLuint fragmentShader;

    std::ifstream is_fs("C:/Users/Joaco/Desktop/Computer Graphics UE/Task_0/ECG_Solution/src/teapotRenderer.frag");// read shader file
    const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>());
    fragmentSource = f_fs.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create an empty vertex shader handle
    glShaderSource(fragmentShader, 1, &fragmentSource, 0);
    glCompileShader(fragmentShader); // Compile the vertex shader

    // Check for fs errors
    GLint succeded_fs;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &succeded_fs);
    if (succeded_fs == GL_FALSE) {
        GLint logSize;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
        GLchar* message = new char[logSize];
        glGetShaderInfoLog(fragmentShader, logSize, NULL, message);
        std::cerr << message;
        delete[] message;
    }

    /* Compile Shader Program */
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindAttribLocation(shaderProgram, 0, "in_Position");
    glBindAttribLocation(shaderProgram, 1, "in_Color");

    glLinkProgram(shaderProgram);

    // check for sp errors
    int IsLinked;
    int maxLength;
    char *shaderProgramInfoLog;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int*)&IsLinked);
    if (IsLinked == FALSE)
    {
        /* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        /* The maxLength includes the NULL character */
        shaderProgramInfoLog = (char*)malloc(maxLength);

        /* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

        std::cerr << shaderProgramInfoLog; // display the error log
        free(shaderProgramInfoLog);
    }
#if _DEBUG
    glDebugMessageCallback(DebugCallback, NULL);// Register the debug callback function.
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Enable synchronous callback. This ensures that your callback function is called right after an error has occurred. 
#endif

    float triangleVertices[] = {
      0.0f,  0.5f, // Vertex 1 (X, Y)
      0.5f, -0.5f, // Vertex 2 (X, Y)
     -0.5f, -0.5f  // Vertex 3 (X, Y)
    };
    
	while(!glfwWindowShouldClose(window)) // render loop
	{

		glClear(GL_COLOR_BUFFER_BIT); // clear screen

        /* Update scene, draw scene  and handle inputs*/

		glfwPollEvents(); // handle OS events

        GLint location = glGetUniformLocation(shaderProgram, "outColor");
        glUseProgram(shaderProgram); // Load the shader into the rendering pipeline 
        glUniform4f(location, 0.3, 0.5, 0.7, 1.0);
        drawTeapot(); // draw the teapot

		glfwSwapBuffers(window); // swap buffer
	}

    /* Free Resources */
    glUseProgram(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
	destroyFramework(); // destroy framework
    glfwDestroyWindow(window);
    glfwTerminate();

	return EXIT_SUCCESS;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
    GLsizei length, const GLchar* message, const GLvoid* userParam)
{
    std::string error = FormatDebugOutput(source, type, id, severity, message);
    std::cout << error << std::endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    std::stringstream stringStream;
    std::string sourceString;
    std::string typeString;
    std::string severityString;

    switch (source) {
    case GL_DEBUG_SOURCE_API: {
        sourceString = "API";
        break;
    }
    case GL_DEBUG_SOURCE_APPLICATION: {
        sourceString = "Application";
        break;
    }
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
        sourceString = "Window System";
        break;
    }
    case GL_DEBUG_SOURCE_SHADER_COMPILER: {
        sourceString = "Shader Compiler";
        break;
    }
    case GL_DEBUG_SOURCE_THIRD_PARTY: {
        sourceString = "Third Party";
        break;
    }
    case GL_DEBUG_SOURCE_OTHER: {
        sourceString = "Other";
        break;
    }
    default: {
        sourceString = "Unknown";
        break;
    }
    }

    switch (type) {
    case GL_DEBUG_TYPE_ERROR: {
        typeString = "Error";
        break;
    }
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
        typeString = "Deprecated Behavior";
        break;
    }
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
        typeString = "Undefined Behavior";
        break;
    }
    case GL_DEBUG_TYPE_PORTABILITY_ARB: {
        typeString = "Portability";
        break;
    }
    case GL_DEBUG_TYPE_PERFORMANCE: {
        typeString = "Performance";
        break;
    }
    case GL_DEBUG_TYPE_OTHER: {
        typeString = "Other";
        break;
    }
    default: {
        typeString = "Unknown";
        break;
    }
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH: {
        severityString = "High";
        break;
    }
    case GL_DEBUG_SEVERITY_MEDIUM: {
        severityString = "Medium";
        break;
    }
    case GL_DEBUG_SEVERITY_LOW: {
        severityString = "Low";
        break;
    }
    default: {
        severityString = "Unknown";
        break;
    }
    }

    stringStream << "OpenGL Error: " << msg;
    stringStream << " [Source = " << sourceString;
    stringStream << ", Type = " << typeString;
    stringStream << ", Severity = " << severityString;
    stringStream << ", ID = " << id << "]";

    return stringStream.str();
}
