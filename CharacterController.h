#pragma once
#include "stdafx.h"
using namespace std;


class CharacterController {
private:
	map<OBJECTid, Character>characterList;
public:
	CharacterController();
	void addCharacter(Character character, CHARACTERid id);
	void deleteCharacter(CHARACTERid id);
	void playAction();
};