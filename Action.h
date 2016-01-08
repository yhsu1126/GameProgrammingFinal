#pragma once
#ifndef FLY2
#define FLY2
#include "string.h"
#include "library.h"
#endif

using namespace std;

class Action {
private:
	ACTIONid id;
	string name;
	string cwaname;
	float speed;
	float range;
public:
    Action(); 
	Action(string name, string cwaname, float speed, float range);
    ~Action();
	
	ACTIONid getId();
	void setId(ACTIONid id);

	string getName();
	void setName(string name);

	string getCwaname();
	void setCwaname(string name);
	
	float getSpeed();
	void setSpeed(float);
	
	float getRange();
	void setRange(float);
};