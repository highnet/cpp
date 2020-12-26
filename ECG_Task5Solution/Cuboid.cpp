#include "Cuboid.h"
Cuboid::Cuboid(glm::mat4 _transform, glm::vec3 _position, float length, float width, float height, float r, float g, float b, float ka, float kd, float ks, int alpha) {

	position = _position;
	transform = glm::translate(_transform, position);
	mesh = CuboidMesh(length, height, width);
	material = Material(r, g, b, ka, kd, ks, alpha);
	texture = Texture("assets/textures/wood_texture.dds");
	glGenVertexArrays(1, &Vao); // create the VAO
	glBindVertexArray(Vao); // bind the VAO
	glGenBuffers(1, &Vbo); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, Vbo); // bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.data), mesh.data, GL_STATIC_DRAW); // buffer the vertex data

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	// color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	// texture coord attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
}
