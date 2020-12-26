#pragma once
#include <glm\detail\type_vec.hpp>
#include <glm\detail\type_mat.hpp>
#include <glm/glm.hpp>


#ifndef  OrbitalCamera_h
#define OrbitalCamera_h

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

#endif /OrbitalCamera_h/