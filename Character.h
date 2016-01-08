#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
using namespace std;

class Character : public FnCharacter {
private:
	string name;
	OBJECTid id;
	ACTIONid curposeID;
	ACTIONid defaultposeID;
	Action actions;
	int remainFrame;
	float normalSpeed, minSpeed, maxSpeed;
	vector<Effect>effectStack;

public:
	Character();
	Character(OBJECTid id);
};