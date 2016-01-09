#pragma once
#include "stdafx.h"

using namespace std;

void CharacterController::addCharacter(Character character,CHARACTERid characterid) {
	characterList[characterid] = character;
}

void CharacterController::deleteCharacter(CHARACTERid characterid) {
	characterList.erase(characterid);
}

void CharacterController::playAction(int skip) {
	for (auto it = characterList.begin(); it != characterList.end(); it++) {
		it->second.Play(LOOP, (float)skip, FALSE, TRUE);
		it->second.Play(LOOP, (float)skip, FALSE, TRUE);
	}
}

void CharacterController :: startPlayAction() {
	for (auto it = characterList.begin(); it != characterList.end(); it++) {
		it->second.Play(START, 0.0f, FALSE, TRUE);
		it->second.Play(LOOP, 0.0f, FALSE, TRUE);
	}
}

CharacterController::CharacterController() {
	;
}