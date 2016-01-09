#pragma once
#include "stdafx.h"

using namespace std;

void CharacterController::addCharacter(Character character,CHARACTERid characterid) {
	characterList[characterid] = character;
}

void CharacterController::deleteCharacter(CHARACTERid characterid) {
	characterList.erase(characterid);
}

void CharacterController::playAction() {
	/*for (auto it = characterList.begin(); it != characterList.end(); it++) {
		it->second
	}*/
}

CharacterController::CharacterController() {
	;
}