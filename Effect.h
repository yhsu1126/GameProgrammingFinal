#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
using namespace std;

class Effect {
private:
	string description;
	float coolDown;
	float currentTime;
	int typeofEffect;
public:
	Effect();
	string getDescription();
	float getCoolDown();
	float getCurrentTime();
	int getTypeofEffect();
	void setDescription(string description);
	void setCoolDown(float coolDown);
	void setCurrentTime(float currentTime);
	void setTypeofEffect(int type);
	void decreaseCooldown(float interval= 1.0f);
};