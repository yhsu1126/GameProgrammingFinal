#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
using namespace std;
class Effect;

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
	void setDescription(string);
	void setCoolDown(float);
	void setCurrentTime(float);
	void setTypeofEffect(int);
	void decreaseCooldown(float);
	void decreateCooldown();
};