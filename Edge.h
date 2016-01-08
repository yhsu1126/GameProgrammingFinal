#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif

class Edge {
private:
	float vertex1[3];
	float vertex2[3];
	float normal[3];
	float dir[3];
	int type;
public:
	Edge(float *vertex1, float *vertex2);
};