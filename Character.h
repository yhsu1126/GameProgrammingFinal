#pragma once
#ifndef _FlyWin32_H_
#define _FlyWin32_H_
#include <iostream>
#include <string>
#include "FlyWin32.h"
#endif

class Character : public FnCharacter;

class Character : public FnCharacter {
private:
	string name;
	OBJECTid id;
	ACTIONid curposeID;
public:
	Character();
	Character(OBJECTid);
};