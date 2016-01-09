#pragma once
#include "stdafx.h"
#include "Action.h"
#include "Effect.h"
using namespace std;

class Character : public FnCharacter {
private:
	string name;
	CHARACTERid id;
	ACTIONid curposeID, idleID, normalID, walkID, strongID, dieID;
	ACTIONid defaultposeID;
	map<ACTIONid,Action> actions;
	int remainFrame;
	float normalSpeed, minSpeed, maxSpeed;
	vector<Effect>effectStack;

public:
	Character();
	Character(string name);
	~Character();
	void initializeAction();
	void addAction(Action a);
	void deleteAction(ACTIONid id);
	void idle();
	string getCurrentPose();
};