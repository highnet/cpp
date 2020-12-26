#include "DirectionalLightSource.h"

DirectionalLightSource::DirectionalLightSource() {

}

DirectionalLightSource::DirectionalLightSource(glm::mat4 transform, glm::vec3 _color, glm::vec3 _direction) {
	color = _color;
	direction = _direction;
}