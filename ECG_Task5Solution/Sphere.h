#pragma once
#include <vector>
#include "SphereMesh.h"
#include <GL\glew.h>
#include "Material.h"
#include "Texture.h"


#ifndef  Sphere_h
#define Sphere_h

class Sphere {
public:
	SphereMesh mesh; // mesh of the cylinder
	glm::mat4 transform; // transform of the cylinder
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Material material;
	Texture texture;
	glm::vec3 position;
	Sphere::Sphere(glm::mat4 transform, float radius, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position, int horizontalSegments, int verticalSegments, int alpha); // cylinder constructor
};

#endif /Sphere_h/