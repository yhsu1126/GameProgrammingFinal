#include "stdafx.h"
#pragma once
#ifndef FLY2
#define FLY2
#include "library.h"
#endif
#include "Action.h"
//big range is 240
//small range is 80
Action::Action(){
	;
}
Action::Action(string name,string cwaname, float speed, float range){
	this->name = name;
	this->cwaname = name;
	this->speed = speed;
	this->range = range;
}
Action::~Action(){
}
//ID
ACTIONid Action::getId(){
	return this->id;
}
void Action::setId(ACTIONid id){
	this->id = id;
}
//SPEED
float Action::getSpeed(){
	return this->speed;
}
void Action::setSpeed(float speed){
	this->speed = speed;
}
//name
string Action::getName(){
	return this->getName;
}
void Action::setName(string name){
	this->name = name;
}

string Action::getCwaname() {
	return this->cwaname;
}

void Action::setCwaname(string name) {
	this->cwaname = name;
}

//Range
float Action::getRange(){
	return this->range;
}
void Action::setRange(float range){
	this->range = range;
}
