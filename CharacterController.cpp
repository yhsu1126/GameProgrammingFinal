#include"CharacterController.h"

using namespace std;

void CharacterController::addCharacter(Character character) {
	characterList[character.GetBaseObject()] = character;
}

void CharacterController::deleteCharacter(OBJECTid id) {
	map<OBJECTid, Character>::iterator it = characterList.find(id);
	characterList.erase(id);
}

void CharacterController::playAction() {
	/*for (auto it = characterList.begin(); it != characterList.end(); it++) {
		it->second
	}*/
}

CharacterController::CharacterController() {
	;
}