#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif

class Map;

class Map {
private:
	int id;
	string name;
	vector<Edge> edges;
	float pos1[3];
	float pos2[3];
	float centerPoint[3];
	float centerPointuDir[3];
	float centerPointfDir[3];
public:
	Map(string); //Map uses String text file. which include the map edges with two vertex as the value
	void addEdge(Edge);
};