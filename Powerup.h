#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif

class Powerup;

class Powerup : public Fncharacter {
private:
	int type;
	string name;
	FnBillboard text;
public:
	int getType();
	string getName();
	FnBillboard getText();
	void setType(int);
	string setName(string);
	FnBillboard setText(FnBillboard);
};