#include "stdafx.h"
#include"Map.h"
Map::Map() {
	;
}
Map::Map(string filename) {
	fstream infile;
	edges.clear();
	infile.open(filename, ios::in);
	infile >> pos1[0] >> pos1[1] >> pos1[2];
	infile >> fDir1[0] >> fDir1[1] >> fDir1[2];
	infile >> pos2[0] >> pos2[1] >> pos2[2];
	infile >> fDir2[0] >> fDir2[1] >> fDir2[2];
	uDir[0] = 0.0;
	uDir[1] = 0.0;
	uDir[2] = 1.0;
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

void Map::constructMap(string filename) {
	fstream infile;
	edges.clear();
	infile.open(filename, ios::in);
	infile >> pos1[0] >> pos1[1] >> pos1[2];
	infile >> fDir1[0] >> fDir1[1] >> fDir1[2];
	infile >> pos2[0] >> pos2[1] >> pos2[2];
	infile >> fDir2[0] >> fDir2[1] >> fDir2[2];
	uDir[0] = 0.0;
	uDir[1] = 0.0;
	uDir[2] = 1.0;
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

void Map::getPos1(float *pos) {
	for (int i = 0; i < 3; i++) {
		pos[i] = pos1[i];
		
	}
	
}

void Map::getPos2(float *pos) {
	for (int i = 0; i < 3; i++) {
		pos[i] = pos2[i];
		
	}
	
}

void Map::getfDir1(float *fDir) {
	for (int i = 0; i < 3; i++) {
		fDir[i] = fDir1[i];
		
	}
	
}

void Map::getfDir2(float *fDir) {
	for (int i = 0; i < 3; i++) {
		fDir[i] = fDir2[i];
		
	}
	
}

void Map::getuDir(float *uDir) {
	for (int i = 0; i < 3; i++) {
		uDir[i] = this->uDir[i];
		
	}
	
}

void Map::getcenterPointPos(float * pos) {
	for (int i = 0; i < 3; i++) {
		pos[i] = this->centerPoint[i];
		
	}
	
}

void Map::getcenterPointfDir(float *fDir) {
	for (int i = 0; i < 3; i++) {
		fDir[i] = this->centerPointfDir[i];
		
	}
	
}

void Map::getcenterPointuDir(float *uDir) {
	for (int i = 0; i < 3; i++) {
		uDir[i] = this->centerPointuDir[i];
		
	}
}