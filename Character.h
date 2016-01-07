#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
using namespace std;

class Character;

class Character : public FnCharacter {
private:
	string name;
	OBJECTid id;
	ACTIONid curposeID;

public:
	Character();
	Character(OBJECTid);
};