/*
  Motor.cpp - Library for controling DC Motor.
  Created by Daler Rahimov.
*/

#include <Arduino.h>
#ifndef DCMotor_h 
#define DCMotor_h 

// Interface class
class Motor{
   public:
		static const int MAX_SPEED = 255;
		static const int MIN_SPEED = 0;

      // pure virtual function providing interface framework.
		virtual void setSpeed(uint8_t speed);
		virtual void setDirection(bool isForward);
		virtual void stop();
		virtual void run();
};


/**
 * This class is spesific for DC Motor Driver, DROK L298 Dual H Bridge Motor 
 * Speed Controller DC 6.5V
 */
class DCMotorDROKL298: public Motor
{
public:
	uint8_t currentSpeed;

	/*!
		@brief	Constructor  
		@param	pwmPin PWM pin number used to comunicate with a motor driver 
		@param	dirOut1Pin First direction pin  
		@param	dirOut2Pin Second direction pin  
		@note 	This is for specific controller that has 2 control pins for direction control 
		for each motor. 
	*/
	DCMotorDROKL298(uint8_t pwmPin, uint8_t dirOut1Pin, uint8_t dirOut2Pin,bool isForwardDirection=true);
	~DCMotorDROKL298();
	void setSpeed(uint8_t speed);
	void setDirection(bool isForward);
	void stop();
	void run();
	void analogWriteNoInterrupts(uint8_t pin, int val);

private:
	uint8_t pwm;
	uint8_t dirOut1;
	uint8_t dirOut2;
};


/**
 * This class is spesific for DC Motor Driver, Cytron 10A Dual Channel Bi-Directional DC 
 * Motor Driver
 */
class DCMotorCytron: public Motor
{
public:
	uint8_t currentSpeed;

	/*!
		@brief	Constructor  
		@param	pwmPin PWM pin number used to comunicate with a motor driver 
		@param	dirOutPin Direction pin  
		@note 	This is for specific controller that has 1 control pins per motor 
	*/
	DCMotorCytron(uint8_t pwmPin, uint8_t dirOutPin, bool isForwardDirection=true);
	~DCMotorCytron();
	void setSpeed(uint8_t speed);
	void setDirection(bool isForward);
	void stop();
	void run();
	void analogWriteNoInterrupts(uint8_t pin, int val);

private:
	uint8_t pwm;
	uint8_t dirOut;
};


#endif
