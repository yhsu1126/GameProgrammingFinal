#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
using namespace std;

class Ball;

class Ball : public FnCharacter {
private:
	string name;
	OBJECTid id;
	ACTIONid curposeID;
	Action actions;
	float speed;
	float size;

public:
	Ball();
	Ball(OBJECTid);
};