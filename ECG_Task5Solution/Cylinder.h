#pragma once
#include <glm\detail\type_mat.hpp>
#include <glm/glm.hpp>
#include <vector>
#include "CylinderMesh.h"
#include "Material.h"
#include "Texture.h"

#ifndef  Cylinder_h
#define Cylinder_h

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
	Cylinder::Cylinder(glm::mat4 transform, float radius, float length, int segments, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position, int alpha); // cylinder constructor
};

#endif /Cylinder_h/