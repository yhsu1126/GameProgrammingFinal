#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
using namespace std;
#include "Character.h"


class CharacterController {
private:
	map<OBJECTid, Character>characterList;
public:
	CharacterController();
	void addCharacter(Character character, CHARACTERid id);
	void deleteCharacter(CHARACTERid id);
	void playAction();
};