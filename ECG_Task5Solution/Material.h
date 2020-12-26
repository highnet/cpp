#pragma once
#include <glm\detail\type_vec.hpp>
#include <glm/glm.hpp>

#ifndef  Material_h
#define Material_h


class Material {
public:
	glm::vec4 baseColor;
	float k_ambient;
	float k_diffuse;
	float k_specular;
	int alpha;
	Material();
	Material(float, float, float, float, float, float, int);
	Material::~Material();
};
#endif /Material_h/
