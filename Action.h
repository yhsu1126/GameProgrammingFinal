#pragma once
#ifndef FLY2
#define FLY2
#include "string.h"
#include "library.h"
#endif

using namespace std;

class Action {
private:
	int id;
	string description;
	float speed;
	float range;
	float coolDown;
	float basicCoolDown;
public:
    Action(); 
	Action(int,string,float,float,float,float);
    ~Action();
	
	int getId();
	void setId(int);
	
	string getDescription();
	void setDescription(string);
	
	float getSpeed();
	void setSpeed(float);
	
	float getRange();
	void setRange(float);
	
	float getCoolDown();
	void setCoolDown(float);
	
	float getBasicCoolDown();
	void setBasicCoolDown(float);
	
	
	
	
	
	
};