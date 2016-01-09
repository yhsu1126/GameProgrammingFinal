#include "ball.h"
using namespace std;
/*
class Ball : public FnCharacter {
private:
	string name;
	OBJECTid id;
	ACTIONid curposeID;
	Action actions;
	float speed;
	float size;

public:
	Ball();
	Ball(OBJECTid);
	Collide()
	Detect();
	Move();
};*/
Ball::Ball(){
}
Ball::Ball()(OBJECTid object){
}
//collision - change the direction of ball
void Collision::Ball(Character character){
	float posCha[3], posBall[3], newDir[3];
	character.GetPosition(posCha);
	GetPosition(posBall);
	for (int i=0; i<3; i++)
		newDir[i] = posBall[i]-posCha[i];
	newDir[2] = 0;
	SetDirection(newDir);
}
void Collision::(Edge edge){
	float dirBall[3], dirNor[3];
	float dot = 2*FyDot(dirBall, edge.normal);
	GetDirection(dirBall);
	for (int i=0; i<3; i++)
		dirBall[i] -=  dot * edge.normal[i]
	SetDirection(dirBall);
}