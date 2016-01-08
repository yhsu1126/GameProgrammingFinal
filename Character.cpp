#include "stdafx.h"
#pragma once
#include "character.h"
using namespace std;

Character::Character() {
	;
}

Character::Character(string name) {
	this->name = name;
}

Character::~Character() {
	;
}

void Character::initializeAction() {
	if (this->name.length() > 0) {
		if (name.compare("Donzo2")) {
			Action tmpAction("Idle","Idle",0,0);
			this->addAction(tmpAction);

		}
		else if (name.compare("Lyubu2")) {
			Action tmpAction("Idle", "Idle", 0, 0);
			this->addAction(tmpAction);
			Action normal("Walk", "Walk", 0, 0);
		}
	}
}

void Character::addAction(Action a) {
	actions[a.getId] = a;
}

void Character::deleteAction(ACTIONid id) {
	actions.erase(id);
}