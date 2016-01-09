#pragma once
#include "stdafx.h"
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
			Action idle("Idle", "Idle", 0.0f, 80.0f);
			char idle_str[100];
			strtoChar(idle_str, idle.getCwaname());
			idleID = this->GetBodyAction(NULL, idle_str);
			idle.setId(idleID);
			this->addAction(idle);
			Action walking("Walk", "Run", 0.0f, 40.0f);
			char walk_str[100];
			strtoChar(walk_str, walking.getCwaname());
			walkID = this->GetBodyAction(NULL, walk_str);
			walking.setId(walkID);
			this->addAction(walking);
			Action normal("NormalAttack", "AttackL1", 0.2f, 240.0f);
			char normal_str[100];
			strtoChar(normal_str, normal.getCwaname());
			normalID = this->GetBodyAction(NULL, normal_str);
			normal.setId(normalID);
			this->addAction(normal);
			Action strong("StrongAttack", "AttackL2", 0.5f, 80.0f);
			char strong_str[100];
			strtoChar(strong_str, strong.getCwaname());
			strongID = this->GetBodyAction(NULL, strong_str);
			strong.setId(strongID);
			this->addAction(strong);
			Action die("Die", "Die", 0.0f, 80.0f);
			char die_str[100];
			strtoChar(die_str, die.getCwaname());
			dieID = this->GetBodyAction(NULL, die_str);
			die.setId(dieID);
			this->addAction(die);

		}
		else if (name.compare("Lyubu2")) {
			Action idle("Idle", "Idle", 0.0f, 80.0f);
			char idle_str[100];
			strtoChar(idle_str, idle.getCwaname());
			idleID = this->GetBodyAction(NULL, idle_str);
			idle.setId(idleID);
			this->addAction(idle);
			Action walking("Walk", "Walk", 0.0f, 40.0f);
			char walk_str[100];
			strtoChar(walk_str, walking.getCwaname());
			walkID = this->GetBodyAction(NULL, walk_str);
			walking.setId(walkID);
			this->addAction(walking);
			Action normal("NormalAttack", "NormalAttack3", 0.2f, 240.0f);
			char normal_str[100];
			strtoChar(normal_str, normal.getCwaname());
			normalID = this->GetBodyAction(NULL, normal_str);
			normal.setId(normalID);
			this->addAction(normal);
			Action strong("StrongAttack", "HeavyAttack1", 0.5f, 80.0f);
			char strong_str[100];
			strtoChar(strong_str, strong.getCwaname());
			strongID = this->GetBodyAction(NULL, strong_str);
			strong.setId(strongID);
			this->addAction(strong);
			Action die("Die", "Die", 0.0f, 80.0f);
			char die_str[100];
			strtoChar(die_str, die.getCwaname());
			dieID = this->GetBodyAction(NULL, die_str);
			die.setId(dieID);
			this->addAction(die);
		}
	}
}

void Character::addAction(Action a) {
	actions[a.getId()] = a;
}

void Character::deleteAction(ACTIONid id) {
	actions.erase(id);
}

void Character::idle() {
	this->SetCurrentAction(NULL, 0, idleID);
	curposeID = idleID;

}

string Character::getCurrentPose() {
	if (curposeID == idleID) {
		return "idle";

	}
	else if (curposeID == walkID) {
		return "walk";

	}
	else if (curposeID == normalID) {
		return "normal";

	}
	else if (curposeID == strongID) {
		return "strong";

	}
	else if (curposeID == dieID) {
		return "die";

	}
}