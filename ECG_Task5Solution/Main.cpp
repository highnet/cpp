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

class Texture {
public:
	GLuint handle;
	Texture::Texture();
	Texture::Texture(string relativeFilePath);
};
Texture::Texture() {

};

Texture::Texture(string relativeFilePath) {
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	DDSImage img = loadDDS(relativeFilePath.c_str());
	glCompressedTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		img.width,
		img.height,
		0, img.size,
		img.data
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

}

class Material {
public:
	glm::vec4 baseColor;
	float k_ambient;
	float k_diffuse;
	float k_specular;
	Material();
	Material(float, float, float, float, float, float);
};

Material::Material() {

}

Material::Material(float r, float g, float b, float ka, float kd, float ks) {
	baseColor.x = r;
	baseColor.y = g;
	baseColor.z = b;
	k_ambient = ka;
	k_diffuse = kd;
	k_specular = ks;
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
	PointLightSource(glm::mat4, glm::vec3, glm::vec3, float, float, float);
	glm::vec3 color;
	glm::vec3 position;
	float attenuation_Constant;
	float attenuation_Linear;
	float attenuation_Quadratic;
};

PointLightSource::PointLightSource() {

}

PointLightSource::PointLightSource(glm::mat4 transform, glm::vec3 _color, glm::vec3 _position, float _attenuation_Constant, float _attenuation_Linear, float _attenuation_Quadratic) {
	color = _color;
	position = _position;
	attenuation_Constant = _attenuation_Constant;
	attenuation_Linear = _attenuation_Linear;
	attenuation_Quadratic = _attenuation_Quadratic;
	mesh = PointLightSourceCubeMesh();
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);
	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices), mesh.vertices, GL_STATIC_DRAW); // buffer the vertex data
	glEnableVertexAttribArray(0); // set position attribute vertex layout  1/2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // set vertex layout 2/2
	glEnableVertexAttribArray(0);
}

//start
class DirectionalLightSource {
public:
	glm::mat4 transform;
	glm::vec3 color;
	glm::vec3 direction;
	DirectionalLightSource();
	DirectionalLightSource(glm::mat4, glm::vec3, glm::vec3);
};

DirectionalLightSource::DirectionalLightSource() {

}

DirectionalLightSource::DirectionalLightSource(glm::mat4 transform, glm::vec3 _color, glm::vec3 _direction) {
	color = _color;
	direction = _direction;
}

//end
class SphereMesh {
public:
	std::vector<float> data; // dynamic vertices array
	SphereMesh(); // default constructor
	SphereMesh(float radius, float segments); // cylinder mesh  constructor
};

SphereMesh::SphereMesh() { // default constructor

}

SphereMesh::SphereMesh(float radius, float segments) {

	std::vector<glm::vec3> sphereVertices;

	for (unsigned int i = 1; i < segments; i++) {
		float verticalAngle = float(i) * glm::pi<float>() / float(segments);
		for (unsigned int j = 0; j < segments; j++) {
			float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(segments);
			glm::vec3 position = glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			);
			sphereVertices.push_back(position);
		}
	}

	// mid top segments
	for (int i = 0; i < segments; i++) {
		glm::vec3 midPointTop = glm::vec3(0.0f, 1.0 * radius, 0.0f);

		glm::vec3 v0 = midPointTop;
		glm::vec3 v1(sphereVertices[i == segments - 1 ? 0 : i + 1].x, sphereVertices[i == segments - 1 ? 0 : i + 1].y, sphereVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v2(sphereVertices[i].x, sphereVertices[i].y, sphereVertices[i].z);

		//triangle 1
		data.push_back(v0.x); //vx
		data.push_back(v0.y); //vy
		data.push_back(v0.z); //vz

		data.push_back(v0.x); //nx
		data.push_back(v0.y); //ny
		data.push_back(v0.z); //nz

		data.push_back(v1.x); //vx
		data.push_back(v1.y); //vy
		data.push_back(v1.z); //vz

		data.push_back(v1.x); //nx
		data.push_back(v1.y); //ny
		data.push_back(v1.z); //nz

		data.push_back(v2.x); //vx
		data.push_back(v2.y); //vy
		data.push_back(v2.z); //vz

		data.push_back(v2.x); //nx
		data.push_back(v2.y); //ny
		data.push_back(v2.z); //nz

	}

	//mid bottom segments
	for (int i = 0; i < segments; i++) {
		glm::vec3 midPointBottom = glm::vec3(0.0f, -1.0 * radius, 0.0f);

		glm::vec3 v0 = midPointBottom;
		glm::vec3 v1(sphereVertices[i].x, -1.0f * sphereVertices[i].y, sphereVertices[i].z);
		glm::vec3 v2(sphereVertices[i == segments - 1 ? 0 : i + 1].x, -1.0f * sphereVertices[i == segments - 1 ? 0 : i + 1].y, sphereVertices[i == segments - 1 ? 0 : i + 1].z);

		//triangle 1
		data.push_back(v0.x); //vx
		data.push_back(v0.y); //vy
		data.push_back(v0.z); //vz

		data.push_back(v0.x); //nx
		data.push_back(v0.y); //ny
		data.push_back(v0.z); //nz

		data.push_back(v1.x); //vx
		data.push_back(v1.y); //vy
		data.push_back(v1.z); //vz

		data.push_back(v1.x); //nx
		data.push_back(v1.y); //ny
		data.push_back(v1.z); //nz

		data.push_back(v2.x); //vx
		data.push_back(v2.y); //vy
		data.push_back(v2.z); //vz

		data.push_back(v2.x); //nx
		data.push_back(v2.y); //ny
		data.push_back(v2.z); //nz

	}

	// rest of segments
	for (int j = 0; j < sphereVertices.size() - segments; j += segments) {
		glm::vec3 v0, v1, v2, v3, v4, v5;
		for (int i = 0; i < segments; i++) {

			if (i < segments - 1) {
				glm::vec3 v0(sphereVertices[i + j].x, sphereVertices[i + j].y, sphereVertices[i + j].z);
				glm::vec3 v1(sphereVertices[segments + i + 1 + j].x, sphereVertices[segments + i + 1 + j].y, sphereVertices[segments + i + 1 + j].z);
				glm::vec3 v2(sphereVertices[segments + i + j].x, sphereVertices[segments + i + j].y, sphereVertices[segments + i + j].z);

				glm::vec3 v3(sphereVertices[i + 1 + j].x, sphereVertices[i + 1 + j].y, sphereVertices[i + 1 + j].z);
				glm::vec3 v5(sphereVertices[i + j].x, sphereVertices[i + j].y, sphereVertices[i + j].z);
				glm::vec3 v4(sphereVertices[segments + i + 1 + j].x, sphereVertices[segments + i + 1 + j].y, sphereVertices[segments + i + 1 + j].z);

				//triangle 1
				data.push_back(v0.x); //vx
				data.push_back(v0.y); //vy
				data.push_back(v0.z); //vz

				data.push_back(v0.x); //nx
				data.push_back(v0.y); //ny
				data.push_back(v0.z); //nz

				data.push_back(v1.x); //vx
				data.push_back(v1.y); //vy
				data.push_back(v1.z); //vz

				data.push_back(v1.x); //nx
				data.push_back(v1.y); //ny
				data.push_back(v1.z); //nz

				data.push_back(v2.x); //vx
				data.push_back(v2.y); //vy
				data.push_back(v2.z); //vz

				data.push_back(v2.x); //nx
				data.push_back(v2.y); //ny
				data.push_back(v2.z); //nz

				//triangle 2
				data.push_back(v3.x); //vx
				data.push_back(v3.y); //vy
				data.push_back(v3.z); //vz

				data.push_back(v3.x); //nx
				data.push_back(v3.y); //ny
				data.push_back(v3.z); //nz

				data.push_back(v4.x); //vx
				data.push_back(v4.y); //vy
				data.push_back(v4.z); //vz

				data.push_back(v4.x); //nx
				data.push_back(v4.y); //ny
				data.push_back(v4.z); //nz

				data.push_back(v5.x); //vx
				data.push_back(v5.y); //vy
				data.push_back(v5.z); //vz

				data.push_back(v5.x); //nx
				data.push_back(v5.y); //ny
				data.push_back(v5.z); //nz

			}
			else {
				glm::vec3 v0(sphereVertices[i + j].x, sphereVertices[i + j].y, sphereVertices[i + j].z);
				glm::vec3 v1(sphereVertices[i + 1 + j].x, sphereVertices[i + 1 + j].y, sphereVertices[i + 1 + j].z);
				glm::vec3 v2(sphereVertices[segments + i + j].x, sphereVertices[segments + i + j].y, sphereVertices[segments + i + j].z);

				glm::vec3 v3(sphereVertices[i - segments + 1 + j].x, sphereVertices[i - segments + 1 + j].y, sphereVertices[i - segments + 1 + j].z);
				glm::vec3 v5(sphereVertices[i + j].x, sphereVertices[i + j].y, sphereVertices[i + j].z);
				glm::vec3 v4(sphereVertices[i + 1 + j].x, sphereVertices[i + 1 + j].y, sphereVertices[i + 1 + j].z);

				//triangle 1
				data.push_back(v0.x); //vx
				data.push_back(v0.y); //vy
				data.push_back(v0.z); //vz

				data.push_back(v0.x); //nx
				data.push_back(v0.y); //ny
				data.push_back(v0.z); //nz

				data.push_back(v1.x); //vx
				data.push_back(v1.y); //vy
				data.push_back(v1.z); //vz

				data.push_back(v1.x); //nx
				data.push_back(v1.y); //ny
				data.push_back(v1.z); //nz

				data.push_back(v2.x); //vx
				data.push_back(v2.y); //vy
				data.push_back(v2.z); //vz

				data.push_back(v2.x); //nx
				data.push_back(v2.y); //ny
				data.push_back(v2.z); //nz

				//triangle 2
				data.push_back(v3.x); //vx
				data.push_back(v3.y); //vy
				data.push_back(v3.z); //vz

				data.push_back(v3.x); //nx
				data.push_back(v3.y); //ny
				data.push_back(v3.z); //nz

				data.push_back(v4.x); //vx
				data.push_back(v4.y); //vy
				data.push_back(v4.z); //vz

				data.push_back(v4.x); //nx
				data.push_back(v4.y); //ny
				data.push_back(v4.z); //nz

				data.push_back(v5.x); //vx
				data.push_back(v5.y); //vy
				data.push_back(v5.z); //vz

				data.push_back(v5.x); //nx
				data.push_back(v5.y); //ny
				data.push_back(v5.z); //nz
			}
		}
	}

}

class Sphere {
public:
	SphereMesh mesh; // mesh of the cylinder
	glm::mat4 transform; // transform of the cylinder
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Material material;
	glm::vec3 position;
	Sphere::Sphere(glm::mat4 transform, float radius, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position, int segments); // cylinder constructor
};

Sphere::Sphere(glm::mat4 _transform, float radius, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position, int segments) {
	mesh = SphereMesh(radius, segments);
	material = Material(r, g, b, ka, kd, ks);
	position = position;
	transform = glm::translate(_transform, position);

	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO

	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, mesh.data.size() * sizeof(float), &mesh.data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // set position attribute vertex layout  1/2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0); // set vertex layout 2/2

	glEnableVertexAttribArray(1); // set color attribute vertex layout 1/2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0); // disable the VAO
}

class CylinderMesh {
public:
	std::vector<float> data; // dynamic vertices array
	CylinderMesh(); // default constructor
	CylinderMesh(float radius, float length, int segments); // cylinder mesh  constructor
};

CylinderMesh::CylinderMesh() { // default constructor

}

CylinderMesh::CylinderMesh(float radius, float height, int segments) {
	float angleIncrement = (M_PI * 2.0f) / segments; // angle between each vertex of the cylinder
	std::vector<glm::vec3> radiusCircleVertices;


	for (int i = 0; i < segments; i++) {
		glm::vec3 circlePosition = glm::vec3(
			cos(i * angleIncrement) * radius,
			0.5f * height,
			sin(i * angleIncrement) * radius

		);
		radiusCircleVertices.push_back(circlePosition);
	}

	// construct top and bottom circles

	for (int i = 0; i < segments; i++) {

		float u = 0.0f;
		float v = 0.0f;
		glm::vec3 midPoint = glm::vec3(0.0f, 1.0f * 0.5f * height, 0.0f);

		//mid point
		data.push_back(midPoint.x); //vx
		data.push_back(midPoint.y); //vy
		data.push_back(midPoint.z); //vz

		data.push_back(0.0f); //nx
		data.push_back(1.0f); //ny
		data.push_back(0.0f); //nz

		u = 0.5f;
		v = 0.5f;
		data.push_back(0.5f); //u
		data.push_back(0.5f); //v

		// first circle vertex
		if (i != segments - 1) {
			data.push_back(radiusCircleVertices[i + 1].x); //vx
			data.push_back(1.0f * radiusCircleVertices[i + 1].y); //vy
			data.push_back(radiusCircleVertices[i + 1].z); //vz

		}
		else {
			data.push_back(radiusCircleVertices[0].x);//vx
			data.push_back(1.0f * radiusCircleVertices[0].y); //vy
			data.push_back(radiusCircleVertices[0].z); //vz
		}

		data.push_back(0.0f); //nx
		data.push_back(1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * (i + 1))) / 2;
		v = (1 + sin(angleIncrement * (i + 1))) / 2;
		data.push_back(u); //u
		data.push_back(v); //v

		//second circle vertex
		data.push_back(radiusCircleVertices[i].x);//vx
		data.push_back(1.0f * radiusCircleVertices[i].y);//vy
		data.push_back(radiusCircleVertices[i].z); //vz

		data.push_back(0.0f); //nx
		data.push_back(1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * i)) / 2;
		v = (1 + sin(angleIncrement * i)) / 2;
		data.push_back(u); //u
		data.push_back(v); //v
	}

	for (int i = 0; i < segments; i++) {

		float u = 0.0f;
		float v = 0.0f;
		glm::vec3 midPoint = glm::vec3(0.0f, -1.0f * 0.5f * height, 0.0f);

		//mid point
		data.push_back(midPoint.x); //vx
		data.push_back(midPoint.y); //vy
		data.push_back(midPoint.z); //vz

		data.push_back(0.0f); //nx
		data.push_back(-1.0f); //ny
		data.push_back(0.0f); //nz

		data.push_back(0.5f); //u
		data.push_back(0.5f); //v

		//second circle vertex
		data.push_back(radiusCircleVertices[i].x);//vx
		data.push_back(-1.0f * radiusCircleVertices[i].y);//vy
		data.push_back(radiusCircleVertices[i].z); //vz

		data.push_back(0.0f); //nx
		data.push_back(-1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * (i))) / 2;
		v = (1 + sin(angleIncrement * (i))) / 2;
		data.push_back(u); //u
		data.push_back(v); //v

		// first circle vertex
		if (i != segments - 1) {
			data.push_back(radiusCircleVertices[i + 1].x); //vx
			data.push_back(-1.0f * radiusCircleVertices[i + 1].y); //vy
			data.push_back(radiusCircleVertices[i + 1].z); //vz

		}
		else {
			data.push_back(radiusCircleVertices[0].x);//vx
			data.push_back(-1.0f * radiusCircleVertices[0].y); //vy
			data.push_back(radiusCircleVertices[0].z); //vz
		}

		data.push_back(0.0f); //nx
		data.push_back(-1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * (i + 1))) / 2;
		v = (1 + sin(angleIncrement * (i + 1))) / 2;
		data.push_back(u); //u
		data.push_back(v); //v
	}

	// 1 side face per segment, each face has 6 vertices
	for (int i = 0; i < segments; i++) {

		glm::vec3 v0(radiusCircleVertices[i].x, radiusCircleVertices[i].y, radiusCircleVertices[i].z);
		glm::vec3 v1(radiusCircleVertices[i == segments - 1 ? 0 : i + 1].x, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].y, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v2(radiusCircleVertices[i].x, -1 * radiusCircleVertices[i].y, radiusCircleVertices[i].z);

		glm::vec3 v3(radiusCircleVertices[i == segments - 1 ? 0 : i + 1].x, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].y, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v4(radiusCircleVertices[i == segments - 1 ? 0 : i + 1].x, -1 * radiusCircleVertices[i == segments - 1 ? 0 : i + 1].y, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v5(radiusCircleVertices[i].x, -1 * radiusCircleVertices[i].y, radiusCircleVertices[i].z);

		glm::vec3 midPoint = glm::vec3(0.0f, 0.5f * height, 0.0f);

		float u = 0.0f;
		float v = 0.0f;

		//triangle 1
		data.push_back(v0.x); //vx
		data.push_back(v0.y); //vy
		data.push_back(v0.z); //vz

		data.push_back(v0.x - midPoint.x); //nx
		data.push_back(v0.y - midPoint.y); //ny
		data.push_back(v0.z - midPoint.z); //nz

		u = (i) / (float)segments;
		v = 1.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v1.x); //vx
		data.push_back(v1.y); //vy
		data.push_back(v1.z); //vz

		data.push_back(v1.x - midPoint.x); //nx
		data.push_back(v1.y - midPoint.y); //ny
		data.push_back(v1.z - midPoint.z); //nz

		u = (i + 1) / (float)segments;
		v = 1.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v2.x); //vx
		data.push_back(v2.y); //vy
		data.push_back(v2.z); //vz

		data.push_back(v2.x - midPoint.x); //nx
		data.push_back(v2.y - -1 * midPoint.y); //ny
		data.push_back(v2.z - midPoint.z); //nz

		u = (i) / (float)segments;
		v = 0.0f;

		data.push_back(u); //u
		data.push_back(v); //v

		//triangle 2

		data.push_back(v3.x); //vx
		data.push_back(v3.y); //vy
		data.push_back(v3.z); //vz

		data.push_back(v3.x - midPoint.x); //nx
		data.push_back(v3.y - midPoint.y); //ny
		data.push_back(v3.z - midPoint.z); //nz

		u = (i + 1) / (float)segments;
		v = 1.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v4.x); //vx
		data.push_back(v4.y); //vy
		data.push_back(v4.z); //vz

		data.push_back(v4.x - midPoint.x); //nx
		data.push_back(v4.y - -1 * midPoint.y); //ny
		data.push_back(v4.z - midPoint.z); //nz

		u = (i + 1) / (float)segments;
		v = 0.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v5.x); //vx
		data.push_back(v5.y); //vy
		data.push_back(v5.z); //vz

		data.push_back(v5.x - midPoint.x); //nx
		data.push_back(v5.y - -1 * midPoint.y); //ny
		data.push_back(v5.z - midPoint.z); //nz

		u = (i) / (float)segments;
		v = 0.0f;
		data.push_back(u); //u
		data.push_back(v); //v
	}

}

class Cylinder {
public:
	CylinderMesh mesh; // mesh of the cylinder
	glm::mat4 transform; // transform of the cylinder
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Material material;
	glm::vec3 position;
	Texture texture;
	Cylinder::Cylinder(glm::mat4 transform, float radius, float length, int segments, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position); // cylinder constructor
};

Cylinder::Cylinder(glm::mat4 _transform, float radius, float length, int segments, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position) {
	mesh = CylinderMesh(radius, length, segments);
	material = Material(r, g, b, ka, kd, ks);
	texture = Texture("assets/textures/tiles_diffuse.dds");
	position = position;
	transform = glm::translate(_transform, position);

	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO

	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, mesh.data.size() * sizeof(float), &mesh.data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // set position attribute vertex layout  1/2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0); // set vertex layout 2/2

	glEnableVertexAttribArray(1); // set color attribute vertex layout 1/2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2); // set color attribute vertex layout 1/2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

}

class CuboidMesh {
public:
	float data[288] = {

		///f0///
		-0.5f,-0.5f,0.5f, 0.0f,0.0f,1.0f, 0.0f,0.0f, // v0
		0.5f,-0.5f,0.5f, 0.0f,0.0f,1.0f, 1.0f,0.0f,// v1
		0.5f,0.5f,0.5f, 0.0f,0.0f,1.0f, 1.0f,1.0f,// v2
		0.5f,0.5f,0.5f, 0.0f,0.0f,1.0f,  1.0,1.0f,// v2
		-0.5f,0.5f,0.5f, 0.0f,0.0f,1.0f,  0.0f,1.0f,// v3 
		-0.5f,-0.5f,0.5f, 0.0f,0.0f,1.0f,  0.0,0.0f,// v0

		///f1///
		0.5f,-0.5f,-0.5f, 0.0f,0.0f,-1.0f,  0.0f,0.0f,// v4
		-0.5f,-0.5f,-0.5f, 0.0f,0.0f,-1.0f, -1.0f,0.0f,// v5
		-0.5f,0.5f,-0.5f, 0.0f,0.0f,-1.0f,  -1.0f,-1.0f,// v6
		-0.5f,0.5f,-0.5f, 0.0f,0.0f,-1.0f,  -1.0f,-1.0f,// v6
		0.5f,0.5f,-0.5f, 0.0f,0.0f,-1.0f,  0.0f,-1.0f,// v7
		0.5f,-0.5f,-0.5f, 0.0f,0.0f,-1.0f,  0.0f,0.0f,// v4

		///f2///
		0.5f,-0.5f,0.5f, 1.0f,0.0f,0.0f, 0.0f,0.0f, // v1 
		0.5f,-0.5f,-0.5f, 1.0f,0.0f,0.0f, 1.0f,0.0f, // v4
		0.5f,0.5f,-0.5f, 1.0f,0.0f,0.0f, 1.0f,1.0f, // v7
		0.5f,0.5f,-0.5f, 1.0f,0.0f,0.0f, 1.0f,1.0f, // v7
		0.5f,0.5f,0.5f, 1.0f,0.0f,0.0f,  0.0f,1.0f,// v2
		0.5f,-0.5f,0.5f, 1.0f,0.0f,0.0f, 0.0f,0.0f, // v1

		///f3///
		-0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f, 0.0f,0.0f,// v5
		-0.5f,-0.5f,0.5f, -1.0f,0.0f,0.0f,  -1.0f,0.0f,// v0
		-0.5f,0.5f,0.5f, -1.0f,0.0f,0.0f,  -1.0f,-1.0f,// v3
		-0.5f,0.5f,0.5f, -1.0f,0.0f,0.0f, -1.0f,-1.0f,// v3
		-0.5f,0.5f,-0.5f, -1.0f,0.0f,0.0f,  0.0f,-1.0f,// v6
		-0.5f,-0.5f,-0.5f, -1.0f,0.0f,0.0f,  0.0f,0.0f,// v5

		///f4///
		-0.5f,0.5f,-0.5f, 0.0f,1.0f,0.0f,  0.0f,0.0f,// v6
		-0.5f,0.5f,0.5f, 0.0f,1.0f,0.0f, 0.0f,1.0f,// v3
		0.5f,0.5f,0.5f, 0.0f,1.0f,0.0f, 1.0f,0.0f,// v2
		0.5f,0.5f,0.5f, 0.0f,1.0f,0.0f,  1.0f,0.0f,// v2
		0.5f,0.5f,-0.5f, 0.0f,1.0f,0.0f,  1.0f,1.0f,// v7
		-0.5f,0.5f,-0.5f, 0.0f,1.0f,0.0f, 0.0f,0.0f,// v6

		///f5///
		-0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f, 0.0f,0.0f,// v5
		0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,  1.0f,0.0f,// v4
		0.5f,-0.5f,0.5f, 0.0f,-1.0f,0.0f,  1.0f,1.0f,// v1
		0.5f,-0.5f,0.5f, 0.0f,-1.0f,0.0f,  1.0f,1.0f,// v1
		-0.5f,-0.5f,0.5f, 0.0f,-1.0f,0.0f,  0.0f,-1.0f,// v0
		-0.5f,-0.5f,-0.5f, 0.0f,-1.0f,0.0f,  0.0f,0.0f,// v5
	};
	CuboidMesh(); // default constructor 
	CuboidMesh(float length, float height, float width); // 
};

CuboidMesh::CuboidMesh() {

}

CuboidMesh::CuboidMesh(float length, float width, float height) {

	int counter = 0;
	for (int i = 0; i < 288; i++) {
		switch (counter)
		{
		case 0:
			data[i] *= height;
			counter++;
			break;
		case 1:
			data[i] *= length;
			counter++;
			break;
		case 2:
			data[i] *= width;
			counter++;
			break;
		case 3:
			counter++;
			break;
		case 4:
			counter++;
			break;
		case 5:
			counter++;
			break;
		case 6:
			counter++;
			break;
		case 7:
			counter = 0;
			break;
		}
	}


}

class Cuboid {
public:
	glm::mat4 transform; // model matrix of the cuboid object
	glm::vec3 position;
	CuboidMesh mesh; // mesh object
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Cuboid::Cuboid(glm::mat4 transform, glm::vec3 position, float length, float width, float heíght, float r, float g, float b, float, float, float); // constructor
	Material material;
	Texture texture;
};

Cuboid::Cuboid(glm::mat4 _transform, glm::vec3 _position, float length, float width, float height, float r, float g, float b, float ka, float kd, float ks) {

	position = _position;
	transform = glm::translate(_transform, position);
	mesh = CuboidMesh(length, height, width);
	material = Material(r, g, b, ka, kd, ks);
	texture = Texture("assets/textures/wood_texture.dds");
	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO
	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.data), mesh.data, GL_STATIC_DRAW); // buffer the vertex data

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// texture coord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}

class OrbitalCamera {
public:
	glm::vec3 cameraPosition; // cartesian position of the camera
	float orbitalRadius; // orbital radius distance 
	float orbitalInclination; // orbital inclination angle
	float orbitalAzimuth; // orbital azimuth angle
	float orbitalSpeed; // orbital speed for the azimuth and inclination changes
	glm::vec3 targetTransformCartesian; // cartesian position of the camera's target
	glm::mat4 projectionMatrix; // projection matrix used by the camera
	float orbitalSpeedZoom;
	OrbitalCamera(glm::vec3, float, float, float, float, glm::vec3, float); // constructor definition
};

OrbitalCamera::OrbitalCamera(glm::vec3 _transformCartesian, float _orbitalRadius, float _orbitalInclination, float _orbitalAzimuth, float _orbitalSpeed, glm::vec3 _targetTransformCartesian, float _orbitalSpeedZoom) // constructor
{
	cameraPosition = _transformCartesian; //glm::vec3
	orbitalRadius = _orbitalRadius; // float
	orbitalInclination = _orbitalInclination; // float
	orbitalAzimuth = _orbitalAzimuth; //float 
	orbitalSpeed = _orbitalSpeed; // float 
	targetTransformCartesian = _targetTransformCartesian; //glm::vec3
	orbitalSpeedZoom = _orbitalSpeedZoom; // float 
}

class Shader {
public:
	GLuint program;
	GLint view;
	GLint proj;
	GLint model;
	GLint materialColor;
	GLint pointLightColor;
	GLint pointLightPosition;
	GLint viewPosition;
	GLint k_ambient;
	GLint k_diffuse;
	GLint k_specular;
	GLint directionalLightColor;
	GLint directionalLightDirection;
	GLint k_constant;
	GLint k_linear;
	GLint k_quadratic;
	GLint textureLocation;
	string type;
	Shader::Shader(string relativePathVert, string relativePathFrag, string _type);


};

Shader::Shader(string relativePathVert, string relativePathFrag, string _type) {
	type = _type;
	// Compile Vertex Shader 
	const char* vertexSource; // create character list
	GLuint vertexShader; // create vertex shader id
	std::ifstream is_vs(relativePathVert); // read shader file
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
	std::ifstream is_fs(relativePathFrag);// read shader file
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

	program = glCreateProgram(); // create program
	glAttachShader(program, vertexShader); // attach shader
	glAttachShader(program, fragmentShader); // attach shader
	glLinkProgram(program); // link program

	// check for sp errors
	int IsLinked;
	int maxLength;
	char* shaderProgramInfoLog;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&IsLinked);
	if (IsLinked == FALSE)
	{
		/* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		/* The maxLength includes the NULL character */
		shaderProgramInfoLog = (char*)malloc(maxLength);

		/* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
		glGetProgramInfoLog(program, maxLength, &maxLength, shaderProgramInfoLog);

		std::cerr << shaderProgramInfoLog; // display the error log
		free(shaderProgramInfoLog);
	}



	// register debug callback
#if _DEBUG
	glDebugMessageCallback(DebugCallback, NULL);// Register the debug callback function.
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Enable synchronous callback. This ensures that your callback function is called right after an error has occurred. 
#endif

		// get shader program uniform/attribute IDs
	view = glGetUniformLocation(program, "view"); // get uniform ID for view matrix
	proj = glGetUniformLocation(program, "proj"); // get uniform ID for projection matrix 
	model = glGetUniformLocation(program, "model"); // get uniform ID for model matrix

	if (type == "phong" || "gourad") {
		materialColor = glGetUniformLocation(program, "materialColor"); // get uniform ID for out-color vector
		viewPosition = glGetUniformLocation(program, "viewPos"); // get uniform ID for 

		pointLightPosition = glGetUniformLocation(program, "pLightPosition"); // get uniform ID for 
		pointLightColor = glGetUniformLocation(program, "pLightColor"); // get uniform ID for 

		directionalLightColor = glGetUniformLocation(program, "dLightColor"); // get uniform ID for 
		directionalLightDirection = glGetUniformLocation(program, "dLightDirection"); // get uniform ID for 

		k_ambient = glGetUniformLocation(program, "k_ambient"); // get uniform ID for 
		k_diffuse = glGetUniformLocation(program, "k_diffuse"); // get uniform ID for 
		k_specular = glGetUniformLocation(program, "k_specular"); // get uniform ID for 

		k_linear = glGetUniformLocation(program, "k_linear"); // get uniform ID for 
		k_constant = glGetUniformLocation(program, "k_constant"); // get uniform ID for 
		k_quadratic = glGetUniformLocation(program, "k_quadratic"); // get uniform ID for 

	}
	if (type == "basic") {
		pointLightColor = glGetUniformLocation(program, "color"); // get uniform ID for 
	}
	if (type == "phong") {
		textureLocation = glGetUniformLocation(program, "colorTexture");
	}

}

struct Vectors { // Shorthand representation of 3D vectors in this engine
	glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 BACK = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 FORWARD = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 LEFT = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);
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
	bool W_KEY_PRESSED = false;
	bool A_KEY_PRESSED = false;
	bool S_KEY_PRESSED = false;
	bool D_KEY_PRESSED = false;
};

void RenderCuboid(Cuboid object, Shader shader, glm::mat4 viewMatrix, OrbitalCamera camera, PointLightSource pLightSource, DirectionalLightSource dLightSource) {
	/////DRAW cuboid1 with phong shader
	glUseProgram(shader.program); // Load the shader into the rendering pipeline 

	glUniformMatrix4fv(shader.view, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // push view matrix to shader
	glUniformMatrix4fv(shader.proj, 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix)); // push projection matrix to shader
	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // push default model matrix to shader

	glUniform3f(shader.viewPosition, camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z); // push color to shader

	glBindVertexArray(object.Vao); //  bind cuboid VAO

	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(object.transform)); // push cuboid transform to shader
	glUniform3f(shader.materialColor, object.material.baseColor.r, object.material.baseColor.g, object.material.baseColor.b); // push color to shader

	glUniform1f(shader.k_constant, pLightSource.attenuation_Constant);
	glUniform1f(shader.k_linear, pLightSource.attenuation_Linear);
	glUniform1f(shader.k_quadratic, pLightSource.attenuation_Quadratic);
	glm::vec3 energy = glm::vec3(pLightSource.color.x, pLightSource.color.y, pLightSource.color.z);

	glUniform3f(shader.pointLightColor, energy.x, energy.y, energy.z); // push color to shader
	glUniform3f(shader.pointLightPosition, pLightSource.position.x, pLightSource.position.y, pLightSource.position.z); // push color to shader

	glUniform3f(shader.directionalLightColor, dLightSource.color.x, dLightSource.color.y, dLightSource.color.z); // push color to shader
	glUniform3f(shader.directionalLightDirection, dLightSource.direction.x, dLightSource.direction.y, dLightSource.direction.z); // push color to shader

	glUniform1f(shader.k_ambient, object.material.k_ambient);
	glUniform1f(shader.k_diffuse, object.material.k_diffuse);
	glUniform1f(shader.k_specular, object.material.k_specular);

	if (shader.type == "phong") {
		int unit = 0;
		glUniform1i(shader.textureLocation, unit);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, object.texture.handle);
	}


	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); // unbind VAO
}

void RenderSphere(Sphere object, Shader shader, glm::mat4 viewMatrix, OrbitalCamera camera, PointLightSource pLightSource, DirectionalLightSource dLightSource) {
	/////DRAW cylinder1 with phong shader

	glUseProgram(shader.program); // Load the shader into the rendering pipeline 

	glUniformMatrix4fv(shader.view, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // push view matrix to shader
	glUniformMatrix4fv(shader.proj, 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix)); // push projection matrix to shader
	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // push default model matrix to shader

	glUniform3f(shader.viewPosition, camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z); // push color to shader

	glBindVertexArray(object.Vao); //  bind cuboid VAO

	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(object.transform)); // push cuboid transform to shader
	glUniform3f(shader.materialColor, object.material.baseColor.r, object.material.baseColor.g, object.material.baseColor.b); // push color to shader

	glUniform1f(shader.k_constant, pLightSource.attenuation_Constant);
	glUniform1f(shader.k_linear, pLightSource.attenuation_Linear);
	glUniform1f(shader.k_quadratic, pLightSource.attenuation_Quadratic);
	glm::vec3 energy = glm::vec3(pLightSource.color.x, pLightSource.color.y, pLightSource.color.z);

	glUniform3f(shader.pointLightColor, energy.x, energy.y, energy.z); // push color to shader
	glUniform3f(shader.pointLightPosition, pLightSource.position.x, pLightSource.position.y, pLightSource.position.z); // push color to shader

	glUniform3f(shader.directionalLightColor, dLightSource.color.x, dLightSource.color.y, dLightSource.color.z); // push color to shader
	glUniform3f(shader.directionalLightDirection, dLightSource.direction.x, dLightSource.direction.y, dLightSource.direction.z); // push color to shader

	glUniform1f(shader.k_ambient, object.material.k_ambient);
	glUniform1f(shader.k_diffuse, object.material.k_diffuse);
	glUniform1f(shader.k_specular, object.material.k_specular);

	glDrawArrays(GL_TRIANGLES, 0, object.mesh.data.size());
	glBindVertexArray(0); // unbind VAO

}

void RenderCylinder(Cylinder object, Shader shader, glm::mat4 viewMatrix, OrbitalCamera camera, PointLightSource pLightSource, DirectionalLightSource dLightSource) {
	/////DRAW cylinder1 with phong shader
	glUseProgram(shader.program); // Load the shader into the rendering pipeline 

	glUniformMatrix4fv(shader.view, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // push view matrix to shader
	glUniformMatrix4fv(shader.proj, 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix)); // push projection matrix to shader
	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // push default model matrix to shader

	glUniform3f(shader.viewPosition, camera.cameraPosition.x, camera.cameraPosition.y, camera.cameraPosition.z); // push color to shader

	glBindVertexArray(object.Vao); //  bind cuboid VAO

	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(object.transform)); // push cuboid transform to shader
	glUniform3f(shader.materialColor, object.material.baseColor.r, object.material.baseColor.g, object.material.baseColor.b); // push color to shader

	glUniform1f(shader.k_constant, pLightSource.attenuation_Constant);
	glUniform1f(shader.k_linear, pLightSource.attenuation_Linear);
	glUniform1f(shader.k_quadratic, pLightSource.attenuation_Quadratic);
	glm::vec3 energy = glm::vec3(pLightSource.color.x, pLightSource.color.y, pLightSource.color.z);

	glUniform3f(shader.pointLightColor, energy.x, energy.y, energy.z); // push color to shader
	glUniform3f(shader.pointLightPosition, pLightSource.position.x, pLightSource.position.y, pLightSource.position.z); // push color to shader

	glUniform3f(shader.directionalLightColor, dLightSource.color.x, dLightSource.color.y, dLightSource.color.z); // push color to shader
	glUniform3f(shader.directionalLightDirection, dLightSource.direction.x, dLightSource.direction.y, dLightSource.direction.z); // push color to shader

	glUniform1f(shader.k_ambient, object.material.k_ambient);
	glUniform1f(shader.k_diffuse, object.material.k_diffuse);
	glUniform1f(shader.k_specular, object.material.k_specular);

	if (shader.type == "phong") {
		int unit = 0;
		glUniform1i(shader.textureLocation, unit);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, object.texture.handle);
	}


	glDrawArrays(GL_TRIANGLES, 0, object.mesh.data.size());
	glBindVertexArray(0); // unbind VAO
}

void RenderPointLightSource(PointLightSource pLightSource, Shader shader, glm::mat4 viewMatrix, OrbitalCamera camera) {
	//////// draw light source with basic shader
	glUseProgram(shader.program); // Load the shader into the rendering pipeline 

	glUniformMatrix4fv(shader.view, 1, GL_FALSE, glm::value_ptr(viewMatrix)); // push view matrix to shader
	glUniformMatrix4fv(shader.proj, 1, GL_FALSE, glm::value_ptr(camera.projectionMatrix)); // push projection matrix to shader
	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // push default model matrix to shader

	glBindVertexArray(pLightSource.Vao);
	glUniformMatrix4fv(shader.model, 1, GL_FALSE, glm::value_ptr(pLightSource.transform)); // push cylinder transform to shader
	glUniform4f(shader.pointLightColor, pLightSource.color.x, pLightSource.color.y, pLightSource.color.z, 1.0); // push color to shader
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); // unbind VAO
	/////
}

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


	//make shaders
	Shader phongShader("assets/PhongShader.vert", "assets/PhongShader.frag", "phong");
	Shader gouradShader("assets/GouradShader.vert", "assets/GouradShader.frag", "gourad");
	Shader basicShader("assets/BasicShader.vert", "assets/BasicShader.frag", "basic");

	// instantiate objects

	// cuboid definition generation
	Cuboid cuboid(
		glm::mat4(1.0f), // starting transform
		glm::vec3(-1.2f, -1.5f, 0.0f), // starting position
		1.5f, // starting length
		1.5f, // starting height
		1.5f, // starting width
		1.0f, // base color r
		1.0f, // base color g
		1.0f, // base color b
		0.05f, // ka 
		0.8f, // kd
		0.5f // ks
	);

	Cylinder cylinder(
		glm::mat4(1.0f), // starting transform
		0.5f, // starting radius
		1.5f, // starting length
		32, // number of segments
		1.0f, // r
		1.0f, // g
		1.0f, // b
		0.05f, // ka 
		0.8f, // kd
		0.5f, // ks
		glm::vec3(1.2f, -1.5f, 0.0f) //starting position
	);


	Sphere sphere(
		glm::mat4(1.0f), // starting transform
		1.0f, // starting radius
		0.0f, // r
		1.0f, // g
		0.0f, // b
		0.1f, // ka 
		0.9f, // kd
		0.3f, // ks
		glm::vec3(-1.2f, 1.0f, 0.0f), //starting position
		32 //segments
	);

	Sphere sphere2(
		glm::mat4(1.0f), // starting transform
		1.0, // starting radius
		1.0f, // r
		0.0f, // g
		0.0f, // b
		0.1f, // ka 
		0.9f, // kd
		0.3f, // ks
		glm::vec3(1.2f, 1.0f, 0.0f), //starting position
		32 //segments
	);


	//generate camera
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f); // set the as background color
	glViewport(0, 0, width, height); // set viewport transform
	OrbitalCamera mainCamera(
		glm::vec3(1.0f, 1.0f, 0.0f), // camera's position transform in cartesian coordinates x,y,z
		6.0f, // orbital radius distance
		0.0f, // orbital inclination angle 
		0.0f, // orbital azimuth angle
		0.05f, // orbital speed for azimuth and inclination angles
		glm::vec3(0.0f, 0.0f, 0.0f), // target's positon transform in cartesian coordinates x,y,z
		0.25f // orbital zoom speed
	); // create orbital camera
	mainCamera.projectionMatrix = glm::perspective(DegreesToRadians(fovy), aspect_ratio, zNear, zFar); // create perspective matrix

	PointLightSource pointLightSource(
		glm::mat4(1.0f), // transform
		glm::vec3(1.0f, 1.0f, 1.0f), //color
		glm::vec3(0.0, 0.0f, 0.0f), // cardinal position
		1.0f, // constant attenuation
		0.4f, // linear attenuation
		0.1f // quadratic attenuation
	);

	pointLightSource.transform = glm::translate(pointLightSource.transform, pointLightSource.position);
	pointLightSource.transform = glm::scale(pointLightSource.transform, glm::vec3(1.0f, 1.0f, 1.0f));

	DirectionalLightSource directionalLightSource(
		glm::mat4(1.0f), //transform
		glm::vec3(0.8f, 0.8f, 0.8f), //color
		glm::vec3(0.0f, -1.0f, -1.0f) //direction
	);

	glEnable(GL_DEPTH_TEST); // enable Z-Depth buffer system

	// render loop
	while (!glfwWindowShouldClose(window)) // render loop
	{
		double time = glfwGetTime(); // get current time
		double deltaTime = time - lastTime; // calculate time since last update

		if (deltaTime >= max_period) { // FPS limiter
			lastTime = time; // reset last time for FPS limiter

		// handle inputs
			glfwPollEvents(); // handle OS events

			float pointLightMovementSpeed = 0.01f;
			if (Input.UP_KEY_PRESSED && Input.LEFT_KEY_PRESSED) {
				pointLightSource.transform = glm::translate(pointLightSource.transform, Vector3.UP * pointLightMovementSpeed);
				pointLightSource.position += Vector3.UP * pointLightMovementSpeed;
			}
			else if (Input.DOWN_KEY_PRESSED && Input.RIGHT_KEY_PRESSED) {
				pointLightSource.transform = glm::translate(pointLightSource.transform, Vector3.DOWN * pointLightMovementSpeed);
				pointLightSource.position += Vector3.DOWN * pointLightMovementSpeed;
			}
			else if (Input.RIGHT_KEY_PRESSED) {
				pointLightSource.transform = glm::translate(pointLightSource.transform, Vector3.RIGHT * pointLightMovementSpeed);
				pointLightSource.position += Vector3.RIGHT * pointLightMovementSpeed;
			}
			else if (Input.LEFT_KEY_PRESSED) {
				pointLightSource.transform = glm::translate(pointLightSource.transform, Vector3.LEFT * pointLightMovementSpeed);
				pointLightSource.position += Vector3.LEFT * pointLightMovementSpeed;
			}
			else if (Input.UP_KEY_PRESSED) {
				pointLightSource.transform = glm::translate(pointLightSource.transform, Vector3.FORWARD * pointLightMovementSpeed);
				pointLightSource.position += Vector3.FORWARD * pointLightMovementSpeed;
			}
			else if (Input.DOWN_KEY_PRESSED) {
				pointLightSource.transform = glm::translate(pointLightSource.transform, Vector3.BACK * pointLightMovementSpeed);
				pointLightSource.position += Vector3.BACK * pointLightMovementSpeed;
			}

			float directionalLightRotationSpeed = 0.01f;
			if (Input.W_KEY_PRESSED) {
				directionalLightSource.direction += glm::vec3(0.0f, directionalLightRotationSpeed, 0.0f);
			}
			if (Input.S_KEY_PRESSED) {
				directionalLightSource.direction -= glm::vec3(0.0f, directionalLightRotationSpeed, 0.0f);
			}
			if (Input.A_KEY_PRESSED) {
				directionalLightSource.direction += glm::vec3(directionalLightRotationSpeed, 0.0f, 0.0f);
			}
			if (Input.D_KEY_PRESSED) {
				directionalLightSource.direction -= glm::vec3(directionalLightRotationSpeed, 0.0f, 0.0f);
			}

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

			mainCamera.cameraPosition = glm::vec3(newCameraX, newCameraY, newCameraZ); // set the camera cartesian transform to the main camera

			glm::mat4 viewMatrix = Camera_LookAt(
				mainCamera.cameraPosition, //eye 
				mainCamera.targetTransformCartesian, // target
				Vector3.UP // up
			); // after being set in the right cartesian position, finally look at the target

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

			glFrontFace(GL_CCW);		// counter clockwise

			RenderPointLightSource(pointLightSource, basicShader, viewMatrix, mainCamera);
			RenderCuboid(cuboid, phongShader, viewMatrix, mainCamera, pointLightSource, directionalLightSource);
			RenderCylinder(cylinder, phongShader, viewMatrix, mainCamera, pointLightSource, directionalLightSource);
			RenderSphere(sphere, gouradShader, viewMatrix, mainCamera, pointLightSource, directionalLightSource);

			glfwSwapBuffers(window); // swap buffer
		}
	}

	/* Free Resources */
	glUseProgram(0);
	glBindVertexArray(0);
	glDeleteProgram(phongShader.program);
	glDeleteProgram(gouradShader.program);

	glDeleteProgram(basicShader.program);

	glDeleteBuffers(1, &cuboid.Vbo);
	glDeleteBuffers(1, &cuboid.Ebo);
	glDeleteVertexArrays(1, &cuboid.Vao);

	glDeleteBuffers(1, &pointLightSource.Vbo);
	glDeleteVertexArrays(1, &pointLightSource.Vao);


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
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		Input.W_KEY_PRESSED = TRUE;
	}
	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		Input.W_KEY_PRESSED = FALSE;
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		Input.A_KEY_PRESSED = TRUE;
	}
	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		Input.A_KEY_PRESSED = FALSE;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		Input.S_KEY_PRESSED = TRUE;
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		Input.S_KEY_PRESSED = FALSE;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		Input.D_KEY_PRESSED = TRUE;
	}
	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		Input.D_KEY_PRESSED = FALSE;
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
