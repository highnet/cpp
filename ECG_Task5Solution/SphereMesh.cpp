#include "SphereMesh.h"

SphereMesh::SphereMesh() { // default constructor

}

SphereMesh::SphereMesh(float radius, float latitudeSegments, float longitudeSegments) {

	std::vector<glm::vec3> sphereVertices;

	for (unsigned int i = 1; i < latitudeSegments; i++) {
		float verticalAngle = float(i) * glm::pi<float>() / float(latitudeSegments);
		for (unsigned int j = 0; j < longitudeSegments; j++) {
			float horizontalAngle = float(j) * 2.0f * glm::pi<float>() / float(longitudeSegments);
			glm::vec3 position = glm::vec3(
				radius * glm::sin(verticalAngle) * glm::cos(horizontalAngle),
				radius * glm::cos(verticalAngle),
				radius * glm::sin(verticalAngle) * glm::sin(horizontalAngle)
			);
			sphereVertices.push_back(position);
		}
	}

	//  top segments
	for (int i = 0; i < longitudeSegments; i++) {

		float u, v;
		glm::vec3 sphereCenter = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 midPointTop = glm::vec3(0.0f, 1.0 * radius, 0.0f);

		glm::vec3 v0 = midPointTop;
		glm::vec3 v1(sphereVertices[i == longitudeSegments - 1 ? 0 : i + 1].x, sphereVertices[i == longitudeSegments - 1 ? 0 : i + 1].y, sphereVertices[i == longitudeSegments - 1 ? 0 : i + 1].z);
		glm::vec3 v2(sphereVertices[i].x, sphereVertices[i].y, sphereVertices[i].z);

		//triangle 1
		data.push_back(v0.x); //vx
		data.push_back(v0.y); //vy
		data.push_back(v0.z); //vz

		data.push_back(v0.x); //nx
		data.push_back(v0.y); //ny
		data.push_back(v0.z); //nz

		u = 0.5 + (atan2(v0.x, v0.z) / (2.0f * glm::pi<float>()));
		v = 0.5 - (asin(v0.y) / glm::pi<float>());

		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v1.x); //vx
		data.push_back(v1.y); //vy
		data.push_back(v1.z); //vz

		data.push_back(v1.x); //nx
		data.push_back(v1.y); //ny
		data.push_back(v1.z); //nz

		u = 0.5 + (atan2(v1.x, v1.z) / (2.0f * glm::pi<float>()));
		v = 0.5 - (asin(v1.y) / glm::pi<float>());

		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v2.x); //vx
		data.push_back(v2.y); //vy
		data.push_back(v2.z); //vz

		data.push_back(v2.x); //nx
		data.push_back(v2.y); //ny
		data.push_back(v2.z); //nz

		u = 0.5 + (atan2(v2.x, v2.z) / (2.0f * glm::pi<float>()));
		v = 0.5 - (asin(v2.y) / glm::pi<float>());

		data.push_back(u); //u
		data.push_back(v); //v

	}

	// bottom segments
	for (int i = 0; i < longitudeSegments; i++) {
		float u, v;

		glm::vec3 midPointBottom = glm::vec3(0.0f, -1.0 * radius, 0.0f);
		glm::vec3 v0 = midPointBottom;
		glm::vec3 v1(sphereVertices[i].x, -1.0f * sphereVertices[i].y, sphereVertices[i].z);
		glm::vec3 v2(sphereVertices[i == longitudeSegments - 1 ? 0 : i + 1].x, -1.0f * sphereVertices[i == longitudeSegments - 1 ? 0 : i + 1].y, sphereVertices[i == longitudeSegments - 1 ? 0 : i + 1].z);

		//triangle 1
		data.push_back(v0.x); //vx
		data.push_back(v0.y); //vy
		data.push_back(v0.z); //vz

		data.push_back(v0.x); //nx
		data.push_back(v0.y); //ny
		data.push_back(v0.z); //nz


		u = 0.5 + (atan2(v0.x, v0.z) / (2.0f * glm::pi<float>()));
		v = 0.5 - (asin(v0.y) / glm::pi<float>());

		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v1.x); //vx
		data.push_back(v1.y); //vy
		data.push_back(v1.z); //vz

		data.push_back(v1.x); //nx
		data.push_back(v1.y); //ny
		data.push_back(v1.z); //nz

		u = 0.5 + (atan2(v1.x, v1.z) / (2.0f * glm::pi<float>()));
		v = 0.5 - (asin(v1.y) / glm::pi<float>());

		data.push_back(u); //u
		data.push_back(v); //v

		data.push_back(v2.x); //vx
		data.push_back(v2.y); //vy
		data.push_back(v2.z); //vz

		data.push_back(v2.x); //nx
		data.push_back(v2.y); //ny
		data.push_back(v2.z); //nz

		u = 0.5 + (atan2(v2.x, v2.z) / (2.0f * glm::pi<float>()));
		v = 0.5 - (asin(v2.y) / glm::pi<float>());

		data.push_back(u); //u
		data.push_back(v); //v

	}

	// side segments
	for (int j = 0; j < sphereVertices.size() - longitudeSegments; j += longitudeSegments) {
		for (int i = 0; i < longitudeSegments; i++) {

			float u = -1.0f;
			float v = -1.0f;

			glm::vec3 v0(sphereVertices[i + j].x, sphereVertices[i + j].y, sphereVertices[i + j].z);
			glm::vec3 v1(sphereVertices[i == longitudeSegments - 1 ? i + 1 + j : longitudeSegments + i + 1 + j].x, sphereVertices[i == longitudeSegments - 1 ? i + 1 + j : longitudeSegments + i + 1 + j].y, sphereVertices[i == longitudeSegments - 1 ? i + 1 + j : longitudeSegments + i + 1 + j].z);
			glm::vec3 v2(sphereVertices[longitudeSegments + i + j].x, sphereVertices[longitudeSegments + i + j].y, sphereVertices[longitudeSegments + i + j].z);

			glm::vec3 v3(sphereVertices[i == longitudeSegments - 1 ? i - longitudeSegments + 1 + j : i + 1 + j].x, sphereVertices[i == longitudeSegments - 1 ? i - longitudeSegments + 1 + j : i + 1 + j].y, sphereVertices[i == longitudeSegments - 1 ? i - longitudeSegments + 1 + j : i + 1 + j].z);
			glm::vec3 v4(sphereVertices[i == longitudeSegments - 1 ? i + 1 + j : longitudeSegments + i + 1 + j].x, sphereVertices[i == longitudeSegments - 1 ? i + 1 + j : longitudeSegments + i + 1 + j].y, sphereVertices[i == longitudeSegments - 1 ? i + 1 + j : longitudeSegments + i + 1 + j].z);
			glm::vec3 v5(sphereVertices[i + j].x, sphereVertices[i + j].y, sphereVertices[i + j].z);


			//triangle 1
			data.push_back(v0.x); //vx
			data.push_back(v0.y); //vy
			data.push_back(v0.z); //vz

			data.push_back(v0.x); //nx
			data.push_back(v0.y); //ny
			data.push_back(v0.z); //nz

			u = 0.5 + (atan2(v0.x, v0.z) / (2.0f * glm::pi<float>()));
			v = 0.5 - (asin(v0.y) / glm::pi<float>());

			data.push_back(i == longitudeSegments - 17 ? 0.0f : u); //u
			data.push_back(i == longitudeSegments - 17 ? 0.0f : v); //v

			data.push_back(v1.x); //vx
			data.push_back(v1.y); //vy
			data.push_back(v1.z); //vz

			data.push_back(v1.x); //nx
			data.push_back(v1.y); //ny
			data.push_back(v1.z); //nz

			u = 0.5 + (atan2(v1.x, v1.z) / (2.0f * glm::pi<float>()));
			v = 0.5 - (asin(v1.y) / glm::pi<float>());

			data.push_back(u); //u
			data.push_back(v); //v

			data.push_back(v2.x); //vx
			data.push_back(v2.y); //vy
			data.push_back(v2.z); //vz

			data.push_back(v2.x); //nx
			data.push_back(v2.y); //ny
			data.push_back(v2.z); //nz

			u = 0.5 + (atan2(v2.x, v2.z) / (2.0f * glm::pi<float>()));
			v = 0.5 - (asin(v2.y) / glm::pi<float>());


			data.push_back(u); //u
			data.push_back(v); //v

			//triangle 2
			data.push_back(v3.x); //vx
			data.push_back(v3.y); //vy
			data.push_back(v3.z); //vz

			data.push_back(v3.x); //nx
			data.push_back(v3.y); //ny
			data.push_back(v3.z); //nz

			u = 0.5 + (atan2(v3.x, v3.z) / (2.0f * glm::pi<float>()));
			v = 0.5 - (asin(v3.y) / glm::pi<float>());


			data.push_back(u); //u
			data.push_back(v); //v



			data.push_back(v4.x); //vx
			data.push_back(v4.y); //vy
			data.push_back(v4.z); //vz

			data.push_back(v4.x); //nx
			data.push_back(v4.y); //ny
			data.push_back(v4.z); //nz

			u = 0.5 + (atan2(v4.x, v4.z) / (2.0f * glm::pi<float>()));
			v = 0.5 - (asin(v4.y) / glm::pi<float>());

			data.push_back(u); //u
			data.push_back(v); //v

			data.push_back(v5.x); //vx
			data.push_back(v5.y); //vy
			data.push_back(v5.z); //vz

			data.push_back(v5.x); //nx
			data.push_back(v5.y); //ny
			data.push_back(v5.z); //nz

			u = 0.5 + (atan2(v5.x, v5.z) / (2.0f * glm::pi<float>()));
			v = 0.5 - (asin(v5.y) / glm::pi<float>());

			data.push_back(u); //u
			data.push_back(v); //v

		}
	}

}