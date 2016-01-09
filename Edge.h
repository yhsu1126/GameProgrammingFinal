#pragma once
#include "stdafx.h"
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