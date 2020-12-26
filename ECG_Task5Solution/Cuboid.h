#pragma once
#include <glm\detail\type_mat.hpp>
#include <glm\detail\type_vec.hpp>
#include "CuboidMesh.h"
#include <GL\glew.h>
#include "Material.h"
#include "Texture.h"


#ifndef  Cuboid_h
#define Cuboid_h

class Cuboid {
public:
	glm::mat4 transform; // model matrix of the cuboid object
	glm::vec3 position;
	CuboidMesh mesh; // mesh object
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	GLuint Ebo; // element buffer object
	Cuboid::Cuboid(glm::mat4 transform, glm::vec3 position, float length, float width, float heíght, float r, float g, float b, float, float, float, int); // constructor
	Material material;
	Texture texture;
};

#endif /Cuboid_h/