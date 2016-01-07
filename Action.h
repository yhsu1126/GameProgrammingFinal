#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif

using namespace std;

class Action;

class Action {
private:
	map<ACTIONid, int> allAction;
	float speed;
	string description;
	float range;
	float coolDown;
	float basicCoolDown;
public:
	map <ACTIONid, int> getAllAction();
	float getSpeed();
	string getDescription();
	float getRange();
	float getCoolDown();
	float getBasicCoolDown();
	void setAllAction(map <ACTIONid, int>);
	void setSpeed(float);
	void setDescription(string);
	void setRange(float);
	void setCoolDown(float);
	void setBasicCoolDown(float);
};