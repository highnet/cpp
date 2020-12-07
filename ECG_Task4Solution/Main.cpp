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
#include <math.h>  
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// Prototypes 
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
int main(int argc, char** argv);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset);
float Clamp(float f, float min, float max);
glm::mat4 Camera_LookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up);
void window_onMouseDown(GLFWwindow* window);
void Window_onMouseRelease();
double DegreesToRadians(double degrees);

#define M_PI std::acos(-1.0)

class Material {
public:
	glm::vec4 baseColor;
	Material();
	Material(float, float, float);
};

Material::Material() {

}

Material::Material(float r, float g, float b) {
	baseColor.x = r;
	baseColor.y = g;
	baseColor.z = b;
}

class PointLightSourceCubeMesh {
public:
	float vertices[108] = {
	 -0.5f,-0.5f,0.5f,// 0
	 0.5f,-0.5f,0.5f,// 1
	 0.5f,0.5f,0.5f,// 2
	 0.5f,0.5f,0.5f,// 2
	 -0.5f,0.5f,0.5f,// 3 
	 -0.5f,-0.5f,0.5f,// 0

	 0.5f,-0.5f,-0.5f,// 4
	 -0.5f,-0.5f,-0.5f,// 5
	 -0.5f,0.5f,-0.5f,// 6
	 -0.5f,0.5f,-0.5f,// 6
	 0.5f,0.5f,-0.5f,// 7
	 0.5f,-0.5f,-0.5f,// 4

	 0.5f,-0.5f,0.5f,// 1 
	 0.5f,-0.5f,-0.5f,// 4
	 0.5f,0.5f,-0.5f,// 7
	 0.5f,0.5f,-0.5f,// 7
	 0.5f,0.5f,0.5f, // 2
	 0.5f,-0.5f,0.5f,// 1

	 -0.5f,-0.5f,-0.5f,// 5
	 -0.5f,-0.5f,0.5f, // 0
	 -0.5f,0.5f,0.5f,// 3
	 -0.5f,0.5f,0.5f,// 3
	 -0.5f,0.5f,-0.5f,// 6
	 -0.5f,-0.5f,-0.5f,// 5

	 -0.5f,0.5f,-0.5f,// 6
	 -0.5f,0.5f,0.5f,// 3
	 0.5f,0.5f,0.5f,// 2
	 0.5f,0.5f,0.5f,// 2
	 0.5f,0.5f,-0.5f,// 7
	 -0.5f,0.5f,-0.5f,// 6

	 -0.5f,-0.5f,-0.5f,// 5
	 0.5f,-0.5f,-0.5f,// 4
	 0.5f,-0.5f,0.5f,// 1
	 0.5f,-0.5f,0.5f,// 1
	 -0.5f,-0.5f,0.5f,// 0
	 -0.5f,-0.5f,-0.5f,// 5
	};
	PointLightSourceCubeMesh();
};

PointLightSourceCubeMesh::PointLightSourceCubeMesh() {

}

class PointLightSource {
public:
	glm::mat4 transform;
	PointLightSourceCubeMesh mesh;
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	PointLightSource();
	PointLightSource(glm::mat4,glm::vec3,glm::vec3, GLint);
	glm::vec3 color;
	glm::vec3 position;
};

PointLightSource::PointLightSource() {

}

PointLightSource::PointLightSource(glm::mat4 transform,glm::vec3 _color, glm::vec3 _position, GLint vertexPositions) {
	color = _color;
	position = _position;
	mesh = PointLightSourceCubeMesh();
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);
	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices), mesh.vertices, GL_STATIC_DRAW); // buffer the vertex data
	glEnableVertexAttribArray(vertexPositions); // set position attribute vertex layout  1/2
	glVertexAttribPointer(vertexPositions, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // set vertex layout 2/2
	glEnableVertexAttribArray(0);
}

// ===CYLINDER===
class CylinderMesh {
public:
	std::vector<float> vertices; // dynamic vertices array
	std::vector<unsigned int> indices; // dynamic indices array
	CylinderMesh(); // default constructor
	CylinderMesh(float radius, float length, int segments); // cylinder mesh  constructor
};

CylinderMesh::CylinderMesh() { // default constructor

}

CylinderMesh::CylinderMesh(float radius, float height, int segments) {

	glm::vec3 bottomCapVertex = glm::vec3(0.0f, -0.5f * height, 0.0f);
	vertices.push_back(bottomCapVertex.x);
	vertices.push_back(bottomCapVertex.y);
	vertices.push_back(bottomCapVertex.z);

	glm::vec3 topCapVertex = glm::vec3(0.0f, 0.5f * height, 0.0f);
	vertices.push_back(topCapVertex.x);
	vertices.push_back(topCapVertex.y);
	vertices.push_back(topCapVertex.z);


	const float angleIncrement = (M_PI * 2.0f) / segments; // angle between each vertex of the cylinder

	for (int i = 0; i < segments; i++) {
		glm::vec3 circlePosition = glm::vec3(
			cos(i * angleIncrement) * radius,
			-height / 2.0f,
			sin(i * angleIncrement) * radius
		);

		// bottom ring vertex
		vertices.push_back(circlePosition.x);
		vertices.push_back((circlePosition.y));
		vertices.push_back(circlePosition.z);

		// top ring vertex
		circlePosition.y = height / 2.0f;
		vertices.push_back(circlePosition.x);
		vertices.push_back((circlePosition.y));
		vertices.push_back(circlePosition.z);

		// bottom face
		indices.push_back(0);
		indices.push_back(2 + i * 2);
		indices.push_back(i == segments - 1 ? 2 : 2 + (i + 1) * 2);

		// top face
		indices.push_back(1);
		indices.push_back(i == segments - 1 ? 3 : 3 + (i + 1) * 2);
		indices.push_back(3 + i * 2);

		// side faces
		indices.push_back(2 + i * 2);
		indices.push_back(i == segments - 1 ? 3 : 3 + (i + 1) * 2);
		indices.push_back(i == segments - 1 ? 2 : 2 + (i + 1) * 2);

		indices.push_back(i == segments - 1 ? 3 : 3 + (i + 1) * 2);
		indices.push_back(2 + i * 2);
		indices.push_back(3 + i * 2);

	}
}

class Cylinder {
public:
	CylinderMesh mesh; // mesh of the cylinder
	glm::mat4 transform; // transform of the cylinder
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Cylinder::Cylinder(glm::mat4, float, float, int, GLint); // cylinder constructor
};

Cylinder::Cylinder(glm::mat4 transform, float radius, float length, int segments, GLint vertexPositions) {
	transform = transform;
	mesh = CylinderMesh(radius, length, segments);

	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO

	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), &mesh.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &Ebo); // generate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo); // bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(float), &mesh.indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(vertexPositions); // set position attribute vertex layout  1/2
	glVertexAttribPointer(vertexPositions, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // set vertex layout 2/2


	glEnableVertexAttribArray(0); // disable the VAO
}

// ==CUBOID==
class CuboidMesh {
public:
	float vertices[24]; // vertices array
	unsigned int indices[36]; // indices array
	CuboidMesh(); // default constructor 
	CuboidMesh(float length, float height, float width); // 
};

CuboidMesh::CuboidMesh() {

}

CuboidMesh::CuboidMesh(float length, float width, float height) {

	// generate unit cuboid vertices coordinates
	vertices[0] = -width / 2.0f;  vertices[1] = -height / 2.0f;   vertices[2] = length / 2.0f; //v0
	vertices[3] = width / 2.0f;   vertices[4] = -height / 2.0f;   vertices[5] = length / 2.0f; //v1
	vertices[6] = width / 2.0f; vertices[7] = height / 2.0f; vertices[8] = length / 2.0f; // v2
	vertices[9] = -width / 2.0f;  vertices[10] = height / 2.0f; vertices[11] = length / 2.0f; // v3
	vertices[12] = width / 2.0f; vertices[13] = -height / 2.0f;  vertices[14] = -length / 2.0f; // v4
	vertices[15] = -width / 2.0f;  vertices[16] = -height / 2.0f;  vertices[17] = -length / 2.0f; // v5
	vertices[18] = -width / 2.0f; vertices[19] = height / 2.0f; vertices[20] = -length / 2.0f;  // v6
	vertices[21] = width / 2.0f;  vertices[22] = height / 2.0f; vertices[23] = -length / 2.0f; // v7

	int breaker = 0;
	for (int i = 0; i < 24; i++) {
		std::cout << vertices[i] << ", ";
		if (breaker++ == 2) {
			std::cout << std::endl;
			breaker = 0;
		}
	}
	// specify faces
	indices[0] = 0;  indices[1] = 1; indices[2] = 2; // face 0
	indices[3] = 2;  indices[4] = 3; indices[5] = 0;

	indices[6] = 4;  indices[7] = 5; indices[8] = 6; // face 1
	indices[9] = 6;  indices[10] = 7; indices[11] = 4;

	indices[12] = 1; indices[13] = 4; indices[14] = 7; // face 2 
	indices[15] = 7; indices[16] = 2; indices[17] = 1;

	indices[18] = 5; indices[19] = 0; indices[20] = 3; // face 3 
	indices[21] = 3; indices[22] = 6; indices[23] = 5;

	indices[24] = 6; indices[25] = 3; indices[26] = 2; // face 4
	indices[27] = 2; indices[28] = 7; indices[29] = 6;

	indices[30] = 5; indices[31] = 4; indices[32] = 1; // face 5
	indices[33] = 1; indices[34] = 0; indices[35] = 5;
}

class Cuboid {
public:
	glm::mat4 transform; // model matrix of the cuboid object
	CuboidMesh mesh; // mesh object
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Cuboid::Cuboid(glm::mat4 transform, float length, float width, float heíght,float r, float g, float b, GLint); // constructor
	Material material;
};

Cuboid::Cuboid(glm::mat4 _transform, float length, float width, float height,float r, float g, float b, GLint vertexPositions) {
	transform = _transform;
	mesh = CuboidMesh(length, height, width);
	material = Material(r, g, b);
	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO
	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices), mesh.vertices, GL_STATIC_DRAW); // buffer the vertex data
	glGenBuffers(1, &Ebo); // generate the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo); // bind the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.indices), mesh.indices, GL_STATIC_DRAW);// buffer the index data

	glEnableVertexAttribArray(vertexPositions); // set position attribute vertex layout  1/2
	glVertexAttribPointer(vertexPositions, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // set vertex layout 2/2

	glEnableVertexAttribArray(0); // disable the VAO

}

class OrbitalCamera {
public:
	glm::vec3 transformCartesian; // cartesian position of the camera
	float orbitalRadius; // orbital radius distance 
	float orbitalInclination; // orbital inclination angle
	float orbitalAzimuth; // orbital azimuth angle
	float orbitalSpeed; // orbital speed for the azimuth and inclination changes
	glm::vec3 targetTransformCartesian; // cartesian position of the camera's target
	glm::mat4 projectionMatrix; // projection matrix used by the camera
	float orbitalSpeedZoom;
	float strafeSpeed;
	OrbitalCamera(glm::vec3, float, float, float, float, glm::vec3, float, float); // constructor definition
};

OrbitalCamera::OrbitalCamera(glm::vec3 _transformCartesian, float _orbitalRadius, float _orbitalInclination, float _orbitalAzimuth, float _orbitalSpeed, glm::vec3 _targetTransformCartesian, float _orbitalSpeedZoom, float _strafeSpeed) // constructor
{
	transformCartesian = _transformCartesian; //glm::vec3
	orbitalRadius = _orbitalRadius; // float
	orbitalInclination = _orbitalInclination; // float
	orbitalAzimuth = _orbitalAzimuth; //float 
	orbitalSpeed = _orbitalSpeed; // float 
	targetTransformCartesian = _targetTransformCartesian; //glm::vec3
	orbitalSpeedZoom = _orbitalSpeedZoom; // float 
	strafeSpeed = _strafeSpeed; // float
}

struct Vectors { // Shorthand representation of 3D vectors in this engine
	glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 BACK = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 FORWARD = glm::vec3(0.0f, 1.0f, 1.0f);
	glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 RIGHT = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 ONE = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ZERO = glm::vec3(0.0f, 0.0f, 0.0f);
};

struct InputManager {
	bool UP_KEY_PRESSED = false;
	bool DOWN_KEY_PRESSED = false;
	bool RIGHT_KEY_PRESSED = false;
	bool LEFT_KEY_PRESSED = false;
	bool SCROLL_UP = false;
	bool SCROLL_DOWN = false;
	bool LEFT_MOUSEBUTTON_PRESSED = false;
	bool RIGHT_MOUSEBUTTON_PRESSED = false;
	double current_mouseX = 0.0;
	double current_mouseY = 0.0;
	double old_mouseX = 0.0;
	double old_mouseY = 0.0;
};

// Global variables 
InputManager Input;
Vectors Vector3;
const double PI = std::acos(-1.0);

bool wireframeMode = false;
bool backFaceCullingMode = false;

// Main 
int main(int argc, char** argv)
{
	// Load settings.ini 
	INIReader reader("assets/settings.ini"); // init reader for ini files
	int width = reader.GetInteger("window", "width", 800); // screen width
	int height = reader.GetInteger("window", "height", 800); // screen height
	double aspect_ratio = (double)width / height; // screen aspect ratio
	int refresh_rate = reader.GetInteger("window", "refresh_rate", 60); // frames per second value
	double max_period = 10 / refresh_rate; // updates per second value
	double lastTime = 0.0; // helper variable for managing FPS
	std::string fullscreen = reader.Get("window", "fullscreen", "false"); // fullscreen pseudo bool
	std::string window_title = reader.Get("window", "title", "ECG 2020"); // window title
	double fovy = reader.GetReal("camera", "fov", 60.0); // field of view
	double zNear = reader.GetReal("camera", "near", 0.1); // perspective near clipping plane
	double zFar = reader.GetReal("camera", "far", 100.0); // perspective far clipping plane

	// Initialize scene 
	if (!glfwInit()) { // initialize GLFW
		std::cerr << "ERROR: GLFW Not Initialized"; // if GLFW is not initialized then deliver Error message... 
		return 0; //...and Exit program
	}

	// create debug context
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); // Create a debug OpenGL context or tell your OpenGL library (GLFW, SDL) to do so.
#endif

	// set glfw window hints and enable or disable OpenGL capabilities. 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // specify OpenGL version, 3 "Major" and "minor" are two components of a single version number, separated by a dot.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL version 4.3 is specified.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GLFW_OPENGL_PROFILE and GLFW_OPENGL_CORE_PROFILE specify which OpenGL profile to create the context for.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // specify a fixed size window
	glfwWindowHint(GLFW_SAMPLES, 4);

	// create window 
	GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), nullptr, nullptr);//Create Window

	if (!window) {
		glfwTerminate(); //Free all resources before exit
		std::cerr << "ERROR: Window could not be created"; // if Window failed to create then deliver Error message... 
		return 0; //...and Exit program
	}

	glfwMakeContextCurrent(window); //make sure that context window is active

	// initialite glew
	glewExperimental = true;  // To force GLEW to load all functions, the variable glewExperimental has to be modified:

	GLenum err = glewInit(); //initialize glew
	if (err != GLEW_OK) {
		std::cerr << "ERROR: GLEW failed to initialize"; // if GLEW failed to initialize then deliver Error message... 
		return 0; //...and Exit program
	}

	// Init ECG framework 
	if (!initFramework()) {
		EXIT_WITH_ERROR("Failed to init framework");
	}

	// Bind Key and mouse Buttons
	glfwSetKeyCallback(window, key_callback); // set callback for keyboard
	glfwSetMouseButtonCallback(window, mouseCallback); // set callback for mouse buttons
	glfwSetScrollCallback(window, scrollCallBack); // set callback for scroll wheel

	// Compile Vertex Shader 
	const char* vertexSource; // create character list
	GLuint vertexShader; // create vertex shader id
	std::ifstream is_vs("assets/teapotRenderer.vert"); // read shader file
	const std::string f_vs((std::istreambuf_iterator<char>(is_vs)), std::istreambuf_iterator<char>()); // string buffer
	vertexSource = f_vs.c_str(); // convert character list to string
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create an empty vertex shader handle
	glShaderSource(vertexShader, 1, &vertexSource, 0); // link source
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

	// Compile Fragment Shader 
	const char* fragmentSource; // create character list 
	GLuint fragmentShader; // create frament shader id
	std::ifstream is_fs("assets/teapotRenderer.frag");// read shader file
	const std::string f_fs((std::istreambuf_iterator<char>(is_fs)), std::istreambuf_iterator<char>()); // string buffer
	fragmentSource = f_fs.c_str(); // conver character list to string
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create an empty vertex shader handle
	glShaderSource(fragmentShader, 1, &fragmentSource, 0); //link source
	glCompileShader(fragmentShader); // Compile the fragment shader

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

	// Compile Shader Program 
	GLuint shaderProgram; // create shader program id
	shaderProgram = glCreateProgram(); // create program
	glAttachShader(shaderProgram, vertexShader); // attach shader
	glAttachShader(shaderProgram, fragmentShader); // attach shader
	glLinkProgram(shaderProgram); // link program

	// check for sp errors
	int IsLinked;
	int maxLength;
	char* shaderProgramInfoLog;
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

	//load default shader program
	glUseProgram(shaderProgram); // Load the shader into the rendering pipeline 

	// register debug callback
#if _DEBUG
	glDebugMessageCallback(DebugCallback, NULL);// Register the debug callback function.
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Enable synchronous callback. This ensures that your callback function is called right after an error has occurred. 
#endif

	// get shader program uniform/attribute IDs
	GLint View = glGetUniformLocation(shaderProgram, "view"); // get uniform ID for view matrix
	GLint Proj = glGetUniformLocation(shaderProgram, "proj"); // get uniform ID for projection matrix 
	GLint Model = glGetUniformLocation(shaderProgram, "model"); // get uniform ID for model matrix
	GLint objectColor = glGetUniformLocation(shaderProgram, "objectColor"); // get uniform ID for out-color vector
	GLint lightColor = glGetUniformLocation(shaderProgram, "lightColor"); // get uniform ID for out-color vector

	GLint vertexPositions = glGetAttribLocation(shaderProgram, "position"); // get attribute ID for vertex position


	// generate cuboid object
	Cuboid cuboid(
		glm::mat4(1.0f), // starting transform
		1.0f, // starting length
		1.0f, // starting height
		1.0f, // starting width
		0.25f,
		0.25f,
		0.25f,
		vertexPositions // attribute ID for vertex position
	);


	/*
	Cylinder cylinder(
		glm::mat4(1.0f), // starting transform
		0.6f, // starting radius
		2.0f, // starting length
		7, // number of segments
		vertexPositions
	);
	*/

	//generate camera
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set the as background color
	glViewport(0, 0, width, height); // set viewport transform
	OrbitalCamera mainCamera(
		glm::vec3(1.0f, 1.0f, 0.0f), // camera's position transform in cartesian coordinates x,y,z
		6.0f, // orbital radius distance
		0.0f, // orbital inclination angle 
		0.0f, // orbital azimuth angle
		0.05f, // orbital speed for azimuth and inclination angles
		glm::vec3(0.0f, 0.0f, 0.0f), // target's positon transform in cartesian coordinates x,y,z
		0.25f, // orbital zoom speed
		0.05f // strafe speed
	); // create orbital camera
	mainCamera.projectionMatrix = glm::perspective(DegreesToRadians(fovy), aspect_ratio, zNear, zFar); // create perspective matrix

	glEnable(GL_DEPTH_TEST); // enable Z-Depth buffer system

	// do pre-rendering object manipulation

	cuboid.transform = glm::rotate(cuboid.transform, (float)DegreesToRadians(45.0), glm::vec3(0.0f, 1.0f, 0.0f));
	cuboid.transform = glm::translate(cuboid.transform, glm::vec3(2.0f, 0.0f, 0.0f));

	// cylinder.transform = glm::translate(cylinder.transform, glm::vec3(-2.0f, 0.0f, 0.0f));


	PointLightSource pointLightSource(
		glm::mat4(1.0f), // transform
		glm::vec3(1.0f, 1.0f, 0.0f), //color
		glm::vec3(1.2f, 1.0f, 2.0f), // position
		vertexPositions
	);

	pointLightSource.transform = glm::translate(pointLightSource.transform, pointLightSource.position);
	pointLightSource.transform = glm::scale(pointLightSource.transform, glm::vec3(0.25f, 0.25f, 0.25f));

	// render loop
	while (!glfwWindowShouldClose(window)) // render loop
	{
		double time = glfwGetTime(); // get current time
		double deltaTime = time - lastTime; // calculate time since last update

		if (deltaTime >= max_period) { // FPS limiter
			lastTime = time; // reset last time for FPS limiter

		// handle inputs
			glfwPollEvents(); // handle OS events

			if (Input.SCROLL_UP) {
				mainCamera.orbitalRadius += mainCamera.orbitalSpeedZoom; // move the camera away from target
			}

			else if (Input.SCROLL_DOWN) {
				mainCamera.orbitalRadius -= mainCamera.orbitalSpeedZoom; // move the camera towards target
			}

			Input.SCROLL_DOWN = false; // reset variable
			Input.SCROLL_UP = false; // reset variable

			glfwGetCursorPos(window, &Input.current_mouseX, &Input.current_mouseY); // get cursor position
			double mouseDX = Input.current_mouseX - Input.old_mouseX; //calculate difference in mouseX and mouseY since last frame
			double mouseDY = Input.current_mouseY - Input.old_mouseY;

			Input.old_mouseX = Input.current_mouseX; // set old mouse x to compare in next frame
			Input.old_mouseY = Input.current_mouseY; // set old mouse y to compare in next frame


			if (Input.RIGHT_MOUSEBUTTON_PRESSED && !Input.LEFT_MOUSEBUTTON_PRESSED) { // while RMB is held and LMB is not held

				/*Calculate the look, right and up vectors relating to the camera transform and the target transform*/
				glm::vec3 camera_look_vector = glm::normalize(mainCamera.targetTransformCartesian - mainCamera.transformCartesian);// Normalize the look vector.
				glm::vec3 camera_right_vector = glm::cross(camera_look_vector, Vector3.UP); // Take the cross product of the vector and the up vector. This gives us the camera relative right vector.
				glm::vec3 camera_up_vector = glm::cross(camera_right_vector, camera_look_vector); // Take the cross product of the right vector and the look vector. This is the camera relative up vector*/


				if (mouseDX < 0) {
					mainCamera.transformCartesian += mainCamera.strafeSpeed * camera_right_vector; // move camera relative right
					mainCamera.targetTransformCartesian += mainCamera.strafeSpeed * camera_right_vector; // target must be moved equally
				}
				else if (mouseDX > 0) {
					mainCamera.transformCartesian -= mainCamera.strafeSpeed * camera_right_vector; // move camera relative left
					mainCamera.targetTransformCartesian -= mainCamera.strafeSpeed * camera_right_vector; // target must me moved equally
				}

				/* Recalculate look,right and up vectors in case they were changed since last calculated */
				camera_look_vector = glm::normalize(mainCamera.targetTransformCartesian - mainCamera.transformCartesian);
				camera_right_vector = glm::cross(camera_look_vector, Vector3.UP);
				camera_up_vector = glm::cross(camera_right_vector, camera_look_vector);

				if (mouseDY < 0) {
					mainCamera.transformCartesian -= mainCamera.strafeSpeed * camera_up_vector; // move camera relative down
					mainCamera.targetTransformCartesian -= mainCamera.strafeSpeed * camera_up_vector; // target must be moved equally
				}
				else if (mouseDY > 0) {
					mainCamera.transformCartesian += mainCamera.strafeSpeed * camera_up_vector; // move camera relative up
					mainCamera.targetTransformCartesian += mainCamera.strafeSpeed * camera_up_vector; // target must be moved equaly
				}
			}

			if (Input.LEFT_MOUSEBUTTON_PRESSED && !Input.RIGHT_MOUSEBUTTON_PRESSED) { // while LMB mouse is held and RMB is not held

				if (mouseDX < 0) {
					mainCamera.orbitalAzimuth += mainCamera.orbitalSpeed; // increase azimuth by azimuth speed
				}
				else if (mouseDX > 0) {
					mainCamera.orbitalAzimuth -= mainCamera.orbitalSpeed; // decrease azimuth by azimuth speed
				}

				if (mouseDY < 0) {
					mainCamera.orbitalInclination -= mainCamera.orbitalSpeed; // increase inclination by inclination speed
				}
				else if (mouseDY > 0) {
					mainCamera.orbitalInclination += mainCamera.orbitalSpeed; // decrease inclination by inclination speed
				}

				mainCamera.orbitalInclination = Clamp(mainCamera.orbitalInclination, -1.5f, 1.5f); // clamp values to avoid gimbal lock
			}

			//handle cameras
			float newCameraZ = mainCamera.orbitalRadius * cos(mainCamera.orbitalInclination) * cos(mainCamera.orbitalAzimuth); // convert spherical coordinate to cartesian coordinates
			float newCameraX = mainCamera.orbitalRadius * cos(mainCamera.orbitalInclination) * sin(mainCamera.orbitalAzimuth); // convert spherical coordinate to cartesian coordinates
			float newCameraY = mainCamera.orbitalRadius * sin(mainCamera.orbitalInclination); // convert spherical coordinate to cartesian coordinates

			mainCamera.transformCartesian = glm::vec3(newCameraX, newCameraY, newCameraZ); // set the camera cartesian transform to the main camera

			glm::mat4 view = Camera_LookAt(
				mainCamera.transformCartesian, //eye 
				mainCamera.targetTransformCartesian, // target
				Vector3.UP // up
			); // after being set in the right cartesian position, finally look at the target

			glUniformMatrix4fv(View, 1, GL_FALSE, glm::value_ptr(view)); // push view matrix to shader
			glUniformMatrix4fv(Proj, 1, GL_FALSE, glm::value_ptr(mainCamera.projectionMatrix)); // push projection matrix to shader
			glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // push default model matrix to shader

		// handle pixel drawing
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen with default color

			GLenum mode;
			if (wireframeMode) {
				mode = GL_LINE;
			}
			else {
				mode = GL_FILL;
			}

			if (backFaceCullingMode) {
				glEnable(GL_CULL_FACE);
			}
			else {
				glDisable(GL_CULL_FACE);
			}
			glPolygonMode(GL_FRONT_AND_BACK, mode);

			glUniform4f(lightColor, pointLightSource.color.x, pointLightSource.color.y, pointLightSource.color.z, 1.0); // push color to shader

			glBindVertexArray(cuboid.Vao); //  bind cuboid VAO
			glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(cuboid.transform)); // push cuboid transform to shader
			glUniform4f(objectColor, cuboid.material.baseColor.r, cuboid.material.baseColor.g, cuboid.material.baseColor.b, 1.0); // push color to shader
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // draw cuboid
			glBindVertexArray(0); // unbind VAO

			/*
			glBindVertexArray(cylinder.Vao); //  bind cylinder VAO
			glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(cylinder.transform)); // push cylinder transform to shader
			glUniform4f(objectColor, 0.0, 1.0, 0.0, 0.0); // push color to shader
			glDrawElements(GL_TRIANGLES, cylinder.mesh.indices.size(), GL_UNSIGNED_INT, 0); // draw cylinder
			glBindVertexArray(0); // unbind VAO
			*/

			glBindVertexArray(pointLightSource.Vao);
			glUniformMatrix4fv(Model, 1, GL_FALSE, glm::value_ptr(pointLightSource.transform)); // push cylinder transform to shader
			glUniform4f(objectColor, pointLightSource.color.x, pointLightSource.color.y, pointLightSource.color.z, 1.0); // push color to shader
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0); // unbind VAO


			glfwSwapBuffers(window); // swap buffer
		}
	}

	/* Free Resources */
	glUseProgram(0);
	glBindVertexArray(0);
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

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		Input.LEFT_MOUSEBUTTON_PRESSED = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		Input.LEFT_MOUSEBUTTON_PRESSED = false;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		Input.RIGHT_MOUSEBUTTON_PRESSED = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		Input.RIGHT_MOUSEBUTTON_PRESSED = false;
	}
}

void scrollCallBack(GLFWwindow* window, double xOffset, double yOffset) {
	if (yOffset > 0) {
		Input.SCROLL_UP = true;
	}
	else {
		Input.SCROLL_UP = false;
	}

	if (yOffset < 0) {
		Input.SCROLL_DOWN = TRUE;
	}
	else {
		Input.SCROLL_DOWN = FALSE;
	}
}

void window_onMouseDown(GLFWwindow* window) {
	Input.LEFT_MOUSEBUTTON_PRESSED = true;
}

void Window_onMouseRelease() {
	Input.LEFT_MOUSEBUTTON_PRESSED = false;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		Input.UP_KEY_PRESSED = TRUE;
	}

	if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
		Input.UP_KEY_PRESSED = FALSE;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		Input.DOWN_KEY_PRESSED = TRUE;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		Input.DOWN_KEY_PRESSED = FALSE;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		Input.LEFT_KEY_PRESSED = TRUE;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		Input.LEFT_KEY_PRESSED = FALSE;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		Input.RIGHT_KEY_PRESSED = TRUE;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		Input.RIGHT_KEY_PRESSED = FALSE;
	}

	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		wireframeMode = !wireframeMode;
	}

	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {

		backFaceCullingMode = !backFaceCullingMode;
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

// The first parameter "eye" specifies the position of the camera
// the second parameter "target" is the point to be centered on-screen
// the third parameter "up" specifies the UP axis of your scene
glm::mat4 Camera_LookAt(glm::vec3 positionCartesian, glm::vec3 targetPositionCartesian, glm::vec3 upVector)
{
	glm::vec3 zaxis = normalize(positionCartesian - targetPositionCartesian);    // The "forward" vector.
	glm::vec3 xaxis = normalize(cross(upVector, zaxis));// The "right" vector.
	glm::vec3 yaxis = cross(zaxis, xaxis);     // The "up" vector.

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	glm::mat4 viewMatrix = {
		glm::vec4(xaxis.x,            yaxis.x,            zaxis.x,       0),
		glm::vec4(xaxis.y,            yaxis.y,            zaxis.y,       0),
		glm::vec4(xaxis.z,            yaxis.z,            zaxis.z,       0),
		glm::vec4(-dot(xaxis, positionCartesian), -dot(yaxis, positionCartesian), -dot(zaxis, positionCartesian),  1)
	};

	return viewMatrix;
}

// the first parameter "value" specifies the floating point value to restrict inside the range defined by the min and max values
// the second parameter "min" specifies the minimum floating point value to compare against
// the third parameter "max" specifies The maximum floating point value to compare against.
float Clamp(float value, float min, float max) {
	return value <= min ? min : value >= max ? max : value;
}

// the parameter "degrees" specifies the degrees to be converted from degrees to radians
double DegreesToRadians(double degrees) {
	return (degrees * PI) / 180;
}
