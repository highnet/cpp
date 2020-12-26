#include "CylinderMesh.h"

CylinderMesh::CylinderMesh() { // default constructor

}

CylinderMesh::CylinderMesh(float radius, float height, int segments) {
	float angleIncrement = (std::acos(-1.0) *2.0f) / segments; // angle between each vertex of the cylinder
	std::vector<glm::vec3> radiusCircleVertices;


	for (int i = 0; i < segments; i++) {
		glm::vec3 circlePosition = glm::vec3(
			cos(i * angleIncrement) * radius,
			0.5f * height,
			sin(i * angleIncrement) * radius

		);
		radiusCircleVertices.push_back(circlePosition);
	}

	// construct top and bottom circles

	for (int i = 0; i < segments; i++) {

		float u = 0.0f;
		float v = 0.0f;
		glm::vec3 midPoint = glm::vec3(0.0f, 1.0f * 0.5f * height, 0.0f);

		//mid point
		data.push_back(midPoint.x); //vx
		data.push_back(midPoint.y); //vy
		data.push_back(midPoint.z); //vz

		data.push_back(0.0f); //nx
		data.push_back(1.0f); //ny
		data.push_back(0.0f); //nz

		u = 0.5f;
		v = 0.5f;
		data.push_back(0.5f); //u
		data.push_back(0.5f); //v

		// first circle vertex
		if (i != segments - 1) {
			data.push_back(radiusCircleVertices[i + 1].x); //vx
			data.push_back(1.0f * radiusCircleVertices[i + 1].y); //vy
			data.push_back(radiusCircleVertices[i + 1].z); //vz

		}
		else {
			data.push_back(radiusCircleVertices[0].x);//vx
			data.push_back(1.0f * radiusCircleVertices[0].y); //vy
			data.push_back(radiusCircleVertices[0].z); //vz
		}

		data.push_back(0.0f); //nx
		data.push_back(1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * (i + 1))) / 2;
		v = (1 + sin(angleIncrement * (i + 1))) / 2;
		data.push_back(u); //u
		data.push_back(v); //v

		//second circle vertex
		data.push_back(radiusCircleVertices[i].x);//vx
		data.push_back(1.0f * radiusCircleVertices[i].y);//vy
		data.push_back(radiusCircleVertices[i].z); //vz

		data.push_back(0.0f); //nx
		data.push_back(1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * i)) / 2;
		v = (1 + sin(angleIncrement * i)) / 2;
		data.push_back(u); //u
		data.push_back(v); //v
	}

	for (int i = 0; i < segments; i++) {

		float u = 0.0f;
		float v = 0.0f;
		glm::vec3 midPoint = glm::vec3(0.0f, -1.0f * 0.5f * height, 0.0f);

		//mid point
		data.push_back(midPoint.x); //vx
		data.push_back(midPoint.y); //vy
		data.push_back(midPoint.z); //vz

		data.push_back(0.0f); //nx
		data.push_back(-1.0f); //ny
		data.push_back(0.0f); //nz

		data.push_back(0.5f); //u
		data.push_back(0.5f); //v

		//second circle vertex
		data.push_back(radiusCircleVertices[i].x);//vx
		data.push_back(-1.0f * radiusCircleVertices[i].y);//vy
		data.push_back(radiusCircleVertices[i].z); //vz

		data.push_back(0.0f); //nx
		data.push_back(-1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * (i))) / 2;
		v = (1 + sin(angleIncrement * (i))) / 2;
		data.push_back(u); //u
		data.push_back(v); //v

		// first circle vertex
		if (i != segments - 1) {
			data.push_back(radiusCircleVertices[i + 1].x); //vx
			data.push_back(-1.0f * radiusCircleVertices[i + 1].y); //vy
			data.push_back(radiusCircleVertices[i + 1].z); //vz

		}
		else {
			data.push_back(radiusCircleVertices[0].x);//vx
			data.push_back(-1.0f * radiusCircleVertices[0].y); //vy
			data.push_back(radiusCircleVertices[0].z); //vz
		}

		data.push_back(0.0f); //nx
		data.push_back(-1.0f); //ny
		data.push_back(0.0f); //nz

		u = (1 + cos(angleIncrement * (i + 1))) / 2;
		v = (1 + sin(angleIncrement * (i + 1))) / 2;
		data.push_back(u); //u
		data.push_back(v); //v
	}


	// 1 side face per segment, each face has 6 vertices
	for (int i = 0; i < segments; i++) {

		glm::vec3 v0(radiusCircleVertices[i].x, radiusCircleVertices[i].y, radiusCircleVertices[i].z);
		glm::vec3 v1(radiusCircleVertices[i == segments - 1 ? 0 : i + 1].x, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].y, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v2(radiusCircleVertices[i].x, -1 * radiusCircleVertices[i].y, radiusCircleVertices[i].z);

		glm::vec3 v3(radiusCircleVertices[i == segments - 1 ? 0 : i + 1].x, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].y, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v4(radiusCircleVertices[i == segments - 1 ? 0 : i + 1].x, -1 * radiusCircleVertices[i == segments - 1 ? 0 : i + 1].y, radiusCircleVertices[i == segments - 1 ? 0 : i + 1].z);
		glm::vec3 v5(radiusCircleVertices[i].x, -1 * radiusCircleVertices[i].y, radiusCircleVertices[i].z);

		glm::vec3 midPoint = glm::vec3(0.0f, 0.5f * height, 0.0f);

		float u = 0.0f;
		float v = 0.0f;

		//triangle 1
		data.push_back(v0.x); //vx
		data.push_back(v0.y); //vy
		data.push_back(v0.z); //vz

		data.push_back(v0.x - midPoint.x); //nx
		data.push_back(v0.y - midPoint.y); //ny
		data.push_back(v0.z - midPoint.z); //nz

		u = (i) / (float)segments;
		v = 1.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v1.x); //vx
		data.push_back(v1.y); //vy
		data.push_back(v1.z); //vz

		data.push_back(v1.x - midPoint.x); //nx
		data.push_back(v1.y - midPoint.y); //ny
		data.push_back(v1.z - midPoint.z); //nz

		u = (i + 1) / (float)segments;
		v = 1.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v2.x); //vx
		data.push_back(v2.y); //vy
		data.push_back(v2.z); //vz

		data.push_back(v2.x - midPoint.x); //nx
		data.push_back(v2.y - -1 * midPoint.y); //ny
		data.push_back(v2.z - midPoint.z); //nz

		u = (i) / (float)segments;
		v = 0.0f;

		data.push_back(u); //u
		data.push_back(v); //v

		//triangle 2

		data.push_back(v3.x); //vx
		data.push_back(v3.y); //vy
		data.push_back(v3.z); //vz

		data.push_back(v3.x - midPoint.x); //nx
		data.push_back(v3.y - midPoint.y); //ny
		data.push_back(v3.z - midPoint.z); //nz

		u = (i + 1) / (float)segments;
		v = 1.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v4.x); //vx
		data.push_back(v4.y); //vy
		data.push_back(v4.z); //vz

		data.push_back(v4.x - midPoint.x); //nx
		data.push_back(v4.y - -1 * midPoint.y); //ny
		data.push_back(v4.z - midPoint.z); //nz

		u = (i + 1) / (float)segments;
		v = 0.0f;
		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v5.x); //vx
		data.push_back(v5.y); //vy
		data.push_back(v5.z); //vz

		data.push_back(v5.x - midPoint.x); //nx
		data.push_back(v5.y - -1 * midPoint.y); //ny
		data.push_back(v5.z - midPoint.z); //nz

		u = (i) / (float)segments;
		v = 0.0f;
		data.push_back(u); //u
		data.push_back(v); //v
	}

}