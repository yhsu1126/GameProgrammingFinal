#include"Map.h"
Map::Map(string filename) {
	fstream infile;
	infile.open(filename, ios::in);
	infile >> pos1[0] >> pos1[1] >> pos1[2];
	infile >> pos2[0] >> pos2[1] >> pos2[2];
	infile >> centerPoint[0] >> centerPoint[1] >> centerPoint[2];
	infile >> centerPointfDir[0] >> centerPointfDir[1] >> centerPointfDir[2];
	infile >> centerPointuDir[0] >> centerPointuDir[1] >> centerPointuDir[2];
	int tmp;
	infile >> tmp;
	for (int i = 0; i < tmp; i++) {
		float vertex1[3], vertex2[3];
		infile >> vertex1[0] >> vertex1[1] >> vertex1[2];
		infile >> vertex2[0] >> vertex2[1] >> vertex2[2];
		addEdge(vertex1, vertex2);
	}
}

void Map::addEdge(Edge edge) {
	edges.push_back(edge);
}

void Map::addEdge(float *vertex1, float *vertex2) {
	Edge tmpedge(vertex1, vertex2);
	addEdge(tmpedge);
}