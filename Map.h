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
	float fDir1[3];
	float fDir2[3];
	float uDir[3];
	float centerPoint[3];
	float centerPointuDir[3];
	float centerPointfDir[3];
public:
	Map();
	Map(string fileName); //Map uses String text file. which include the map edges with two vertex as the value
	void constructMap(string fileName);
	void addEdge(Edge edge);
	void addEdge(float *vertex1, float *vertex2);
	void getPos1(float *pos);
	void getPos2(float *pos);
	void getfDir1(float *fDir);
	void getfDir2(float *fDir);
	void getuDir(float *uDir);
	void getcenterPointPos(float *pos);
	void getcenterPointfDir(float *fDir);
	void getcenterPointuDir(float *uDir);
};