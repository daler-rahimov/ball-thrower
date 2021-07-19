/*
  Motor.cpp - Library for controling DC Motor.
  Created by Daler Rahimov.
*/

#include "Arduino.h"
#include "DCMotor.h"

DCMotor::DCMotor(uint8_t pwmPin, uint8_t dirOut1Pin, uint8_t dirOut2Pin,bool isForwardDirection=true)
{
	pwm = pwmPin;
	dirOut1 = dirOut1Pin;
	dirOut2 = dirOut2Pin;
	currentSpeed = 0; 

	// set all the motor control pins to outputs
	pinMode(pwm, OUTPUT);
	pinMode(dirOut1, OUTPUT);
	pinMode(dirOut2, OUTPUT);
	// turn forward or backward direction  
	setDirection(isForwardDirection);
}

DCMotor::~DCMotor()
{
	stop();
}

void DCMotor::setDirection(bool isForward) {
	// turn forward or backward direction  
	if (isForward){
		digitalWrite(dirOut1, HIGH);
		digitalWrite(dirOut2, LOW);
	}else{ 
		digitalWrite(dirOut1, LOW);
		digitalWrite(dirOut2, HIGH);
	}
}


void DCMotor::stop() {
	// turn off motors
	digitalWrite(pwm, LOW);
}


void DCMotor::setSpeed(uint8_t speed) {
	if (speed <= currentSpeed) {
		currentSpeed = speed;
		analogWrite(pwm, currentSpeed);
	}
	else { // speed up slowly so motor does not staul
		for (int i = currentSpeed; i <= speed; i++)
		{
			analogWrite(pwm, i);
			delay(20);
		}
		currentSpeed = speed;
	}
}

void DCMotor::run() {
	analogWrite(pwm, currentSpeed);
}
