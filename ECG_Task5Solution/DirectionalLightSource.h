#pragma once
#include <glm\detail\type_mat.hpp>
#include <glm/glm.hpp>

#ifndef  DirectionalLightSource_h
#define DirectionalLightSource_h

class DirectionalLightSource {
public:
	glm::mat4 transform;
	glm::vec3 color;
	glm::vec3 direction;
	DirectionalLightSource();
	DirectionalLightSource(glm::mat4, glm::vec3, glm::vec3);
};

#endif /DirectionalLightSource_h/