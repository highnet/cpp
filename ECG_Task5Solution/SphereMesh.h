#pragma once
#include <glm\detail\type_mat.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <glm\gtc\constants.hpp>

#ifndef  SphereMesh_h
#define SphereMesh_h

class SphereMesh {
public:
	std::vector<float> data; // dynamic vertices array
	SphereMesh(); // default constructor
	SphereMesh(float radius, float latitudeSegments, float longitudeSegments); // cylinder mesh  constructor
};

#endif /SphereMesh_h/