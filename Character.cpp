#pragma once
#ifndef _FlyWin32_H_
#define _FlyWin32_H_
#include <iostream>
#include <string>
#include "FlyWin32.h"
#endif
#include "character.h"
using namespace std;

Character::Character() {
	;
}

Character::Character(OBJECTid id) {
	this->id = id;
}