#include "OrbitalCamera.h"

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
