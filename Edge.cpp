#include "stdafx.h"
#include "Edge.h"
Edge::Edge(float *v1, float *v2) {
	for (int i = 0; i < 3; i++) {
		vertex1[i] = v1[i];
		vertex2[i] = v2[i];
	}
	dir[2] = 0.0f;
	dir[0] = vertex1[0] - vertex2[0];
	dir[1] = vertex1[1] - vertex2[1];
	normal[2] = 0.0f;
	normal[0] = -1 * dir[1];
	normal[1] = dir[0];
	type = 1;
}