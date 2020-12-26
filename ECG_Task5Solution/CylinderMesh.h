#pragma once
#include <glm\detail\type_mat.hpp>
#include <glm/glm.hpp>
#include <vector>

#ifndef  CylinderMesh_h
#define CylinderMesh_h

class CylinderMesh {
public:
	std::vector<float> data; // dynamic vertices array
	CylinderMesh(); // default constructor
	CylinderMesh(float radius, float length, int segments); // cylinder mesh  constructor
};

#endif /CylinderMesh_h/