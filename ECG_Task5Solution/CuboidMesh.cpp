#include "CuboidMesh.h"
CuboidMesh::CuboidMesh() {

}

CuboidMesh::CuboidMesh(float length, float width, float height) {

	int counter = 0;
	for (int i = 0; i < 288; i++) {
		switch (counter)
		{
		case 0:
			data[i] *= height;
			counter++;
			break;
		case 1:
			data[i] *= length;
			counter++;
			break;
		case 2:
			data[i] *= width;
			counter++;
			break;
		case 3:
			counter++;
			break;
		case 4:
			counter++;
			break;
		case 5:
			counter++;
			break;
		case 6:
			counter++;
			break;
		case 7:
			counter = 0;
			break;
		}
	}


}