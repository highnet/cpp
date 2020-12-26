#include "PointLightSource.h"

PointLightSource::PointLightSource() {

}

PointLightSource::PointLightSource(glm::mat4 transform, glm::vec3 _color, glm::vec3 _position, float _attenuation_Constant, float _attenuation_Linear, float _attenuation_Quadratic) {
	color = _color;
	position = _position;
	attenuation_Constant = _attenuation_Constant;
	attenuation_Linear = _attenuation_Linear;
	attenuation_Quadratic = _attenuation_Quadratic;
	mesh = BasicCubeMesh();
	glGenVertexArrays(1, &Vao);
	glBindVertexArray(Vao);
	glGenBuffers(1, &Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.vertices), mesh.vertices, GL_STATIC_DRAW); // buffer the vertex data
	glEnableVertexAttribArray(0); // set position attribute vertex layout  1/2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); // set vertex layout 2/2
	glEnableVertexAttribArray(0);
}

PointLightSource::~PointLightSource() {

}