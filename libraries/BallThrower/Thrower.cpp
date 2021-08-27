/*
  Thrower.cpp - Library for controling ball Thrower.
  Created by Daler Rahimov
*/

#include "Thrower.h"

Thrower::Thrower(Motor &upMotor, Motor &downMotor)
{
	_downMotorThrower = &downMotor; 
	_upMotorThrower = &upMotor;
}

Thrower::~Thrower()
{
	stopShot();
}

void Thrower::playShot(){
	_downMotorThrower->run();
	_upMotorThrower->run();
}

void Thrower::setShotPower(double shotPowerPersentile = 1.0){
	if (shotPowerPersentile > 1){
		shotPowerPersentile = 1;
	}else if (shotPowerPersentile < 0){
		shotPowerPersentile = 0;
	}
	shotPowerInPersentile = shotPowerPersentile;
	ballMotorPower = _upMotorThrower->MAX_SPEED * shotPowerPersentile;
	// incase setShot not called after setShotPower
	setShot(_currentShot);
}


double Thrower::getShotPower(){
	return shotPowerInPersentile;
}

void Thrower::setSpin(double spinPersentile = 0.0){
	// TODO: impement it 
}

double Thrower::getSpin(){
	// TODO: impement it 
}

void Thrower::stopShot(){
	_downMotorThrower->stop();
	_upMotorThrower->stop();
}

void Thrower::flatShot(){
	_downMotorThrower->setSpeed(ballMotorPower);
	_upMotorThrower->setSpeed(ballMotorPower);
}

void Thrower::topspinShot(){
	_downMotorThrower->setSpeed(ballMotorPower*0.6);
	_upMotorThrower->setSpeed(ballMotorPower);
}
bool Thrower::isModeImplemented(int shot) {
	if (
		shot == Thrower::FLAT_SHOT ||
		shot == Thrower::TOPSPIN_SHOT
		) 
	{
		return true; 
	}
	else {
		return false;
	}
}

int Thrower::setShot(int shot)
{
	// if (_currentShot == shot){
	// 	return 0;
	if(!isModeImplemented(shot)){ // not implemented yet modes 
		return -1;
	}else{// set correct shot 
		switch (shot) {
		case Thrower::FLAT_SHOT:
			flatShot();
			break;
		case Thrower::TOPSPIN_SHOT:
			topspinShot();
			break;
		// case Thrower::DROP_SHOT:
		// 	dropShot();
		// 	break;
		// case Thrower::PASSING_SHOT:
		// 	topspinShot();
		// 	break;
		default:
			flatShot();
			break;
		}
		_currentShot = shot;
		return _currentShot; 
	}
}

int Thrower::getPlayingShot() {
	return _currentShot;
}
