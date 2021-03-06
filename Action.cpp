#pragma once
#include "stdafx.h"
//big range is 240
//small range is 80
Action::Action(){
	;
}
Action::Action(string name, string cwaname, float speed, float range) {
	this->id = id;
	this->name = name;
	this->cwaname = name;
	this->speed = speed;
	this->range = range;
	this->length = -1;
}
Action::~Action(){
	this->length = -1;
}
//ID
ACTIONid Action::getId() {
	return this->id;
}
void Action::setId(ACTIONid id) {
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
	return this->name;
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

// get the length of the action
int Action::getLength() {
	return this->length;
}
void Action::setLength(int length) {
	this->length = length;
}