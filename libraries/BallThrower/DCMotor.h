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

class DCMotor: public Motor
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
	DCMotor(uint8_t pwmPin, uint8_t dirOut1Pin, uint8_t dirOut2Pin,bool isForwardDirection=true);
	~DCMotor();
	void setSpeed(uint8_t speed);
	void setDirection(bool isForward);
	void stop();
	void run();

private:
	uint8_t pwm;
	uint8_t dirOut1;
	uint8_t dirOut2;
};


#endif
