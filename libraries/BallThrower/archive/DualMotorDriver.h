/*
  DualMotorDriver.cpp - Library for driving 2 motors. 
  Control Name: DC Motor Driver, DROK L298 Dual H Bridge Motor Speed Controller 
  
  Created by Daler Rahimov
*/

#ifndef DualMotorDriver_h
#define DualMotorDriver_h

#include <DCMotor.h>
#include "Arduino.h"



class DualMotorDriver
{
public:

    ~DualMotorDriver();
    /*!
		@brief	Constructor 
		@param	pwmDownPin	PWM pin for bottom motor thrower 
		@param	pwmUpPin	PWM pin for top motor thrower 
		@param	directionDownPin1	pin for controlling direction of bottom DC motor 	
		@param	directionDownPin2	pin for controlling direction of bottom DC motor 	
		@param	directionUpPin1	pin for controlling direction of top DC motor 	
		@param	directionUpPin2	pin for controlling direction of top DC motor 	

	*/
    DualMotorDriver(uint8_t pwmDownPin, 
                    uint8_t pwmUpPin, 
                    uint8_t directionDownPin1, 
                    uint8_t directionDownPin2,
                    uint8_t directionUpPin1,
                    uint8_t directionUpPin2
                    );
    /*!
		@brief	Return current shot being played 
		@returns int value of current shot being played 
	*/
    void setSpeedUp(uint8_t speed);
    void setSpeedDown(uint8_t speed);
    void setDirectionUp(bool isForward);
    void setDirectionDown(bool isForward);
    void stopUp();
    void stopDown();



private:
    DCMotor *_downMotorThrower;
    DCMotor *_upMotorThrower;
};

#endif
