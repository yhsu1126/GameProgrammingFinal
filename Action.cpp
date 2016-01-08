#pragma once
#ifndef FLY2
#define FLY2
#include "Action.h"
#endif
Action::Action(){
	this->id = -1;
	this->description = NULL;
	this->speed = 0.0;
	this->range = 0.0;
	this->coolDown = 0.0;
	this->basicCoolDown;
}
Action::Action(int id, string description, float speed, 
	float range, float coolDown,float basicCoolDown){
	this->id = id;
	this->description = new string(description);
	this->speed = speed;
	this->range = range;
	this->coolDown = coolDown;
	this->basicCoolDown = basicCoolDown;
}
Action::~Action(){
	this->setDescription(NULL);
}
//ID
int Action::getId(){
	return this->id;
}
void Action::setId(int id){
	this->id = id;
}
//SPEED
float Action::getSpeed(){
	return this->speed;
}
void Action::setSpeed(float speed){
	this->speed = speed;
}
//DESCRIPTION
string Action::getDescription(){
	return this->description;
}
void Action::setDescription(string des){
	if (this->description!=NULL)
		delete this->description;
	this->description = new string(des);
}
//Range
float Action::getRange(){
	return this->range;
}
void Action::setRange(float range){
	this->range = range;
}
//CoolDown
float Action::getCoolDown(){
	return this->coolDown;
}
void Action::setCoolDown(float coolDown){
	this->coolDown = coolDown;
}
//BasicCoolDown
float Action::getBasicCoolDown(){
	return this->basicCoolDown;
}
void Action::setBasicCoolDown(float basicCoolDown){
	this->basicCoolDown = basicCoolDown;
}
