/*
  Motor.cpp - Library for controling DC Motor.
  Created by Daler Rahimov.
*/

#include "Arduino.h"
#include "DCMotor.h"

//****************************************************
// DCMotorDROKL298
//****************************************************
DCMotorDROKL298::DCMotorDROKL298(uint8_t pwmPin, uint8_t dirOut1Pin, uint8_t dirOut2Pin, 
					bool isForwardDirection = true)
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

DCMotorDROKL298::~DCMotorDROKL298()
{
	stop();
}

void DCMotorDROKL298::setDirection(bool isForward)
{
	// turn forward or backward direction
	if (isForward)
	{
		digitalWrite(dirOut1, HIGH);
		digitalWrite(dirOut2, LOW);
	}
	else
	{
		digitalWrite(dirOut1, LOW);
		digitalWrite(dirOut2, HIGH);
	}
}

void DCMotorDROKL298::stop()
{
	// turn off motors
	digitalWrite(pwm, LOW);
}

void DCMotorDROKL298::setSpeed(uint8_t speed)
{
	if (speed <= currentSpeed)
	{
		currentSpeed = speed;
		analogWrite(pwm, currentSpeed);
	}
	else
	{ // speed up slowly so motor does not staul
		for (int i = currentSpeed; i <= speed; i++)
		{
			analogWrite(pwm, i);
			delay(10);
		}
		currentSpeed = speed;
	}
}

void DCMotorDROKL298::run()
{
	setSpeed(currentSpeed);
}

//****************************************************
// DCMotorCytron
//****************************************************
DCMotorCytron::DCMotorCytron(uint8_t pwmPin, uint8_t dirOutPin, bool isForwardDirection = true)
{
	pwm = pwmPin;
	dirOut = dirOutPin;
	currentSpeed = 0;

	// set all the motor control pins to outputs
	pinMode(pwm, OUTPUT);
	pinMode(dirOut, OUTPUT);
	// turn forward or backward direction
	setDirection(isForwardDirection);
}

DCMotorCytron::~DCMotorCytron()
{
	stop();
}

void DCMotorCytron::setDirection(bool isForward)
{
	// turn forward or backward direction
	if (isForward)
	{
		digitalWrite(dirOut, HIGH);
	}
	else
	{
		digitalWrite(dirOut, LOW);
	}
}

void DCMotorCytron::stop()
{
	// turn off motors
	digitalWrite(pwm, LOW);
}

void DCMotorCytron::setSpeed(uint8_t speed)
{
	if (speed <= currentSpeed)
	{
		currentSpeed = speed;
		analogWrite(pwm, currentSpeed);
	}
	else
	{ // speed up slowly so motor does not staul
		for (int i = currentSpeed; i <= speed; i++)
		{
			analogWrite(pwm, i);
			delay(10);
		}
		currentSpeed = speed;
	}
}

void DCMotorCytron::run()
{
	setSpeed(currentSpeed);
}