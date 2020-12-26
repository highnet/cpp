#include "Cylinder.h"


Cylinder::Cylinder(glm::mat4 _transform, float radius, float length, int segments, float r, float g, float b, float ka, float kd, float ks, glm::vec3 position, int alpha) {
	mesh = CylinderMesh(radius, length, segments);
	material = Material(r, g, b, ka, kd, ks, alpha);
	texture = Texture("assets/textures/tiles_diffuse.dds");
	position = position;
	transform = glm::translate(_transform, position);

	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO

	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, mesh.data.size() * sizeof(float), &mesh.data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // set position attribute vertex layout  1/2
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0); // set vertex layout 2/2

	glEnableVertexAttribArray(1); // set color attribute vertex layout 1/2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2); // set color attribute vertex layout 1/2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

}
