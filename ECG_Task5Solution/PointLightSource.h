#pragma once
#include <glm\detail\type_mat.hpp>
#include <glm/glm.hpp>
#include "BasicCubeMesh.h"
#include <GL\glew.h>


#ifndef PointLightSource_h
#define PointLightSource_h

class PointLightSource {
public:
	glm::mat4 transform;
	BasicCubeMesh mesh;
	GLuint Vao; // vertex array object
	GLuint Vbo; // vertex buffer object
	PointLightSource();
	PointLightSource(glm::mat4, glm::vec3, glm::vec3, float, float, float);
	glm::vec3 color;
	glm::vec3 position;
	float attenuation_Constant;
	float attenuation_Linear;
	float attenuation_Quadratic;
	PointLightSource::~PointLightSource();
};

#endif /PointLightSource_h/