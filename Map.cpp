#ifndef FLY2
#define FLY2
#include "library.h"
#endif
#include"Map.h"
Map::Map(string filename) {

}

void Map::addEdge(Edge edge) {
	edges.push_back(edge);
}

void Map::addEdge(float *vertex1, float *vertex2) {
	//Edge edge.
}