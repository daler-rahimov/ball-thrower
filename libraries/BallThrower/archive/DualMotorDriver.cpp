#include "DualMotorDriver.h"

DualMotorDriver::DualMotorDriver(uint8_t pwmDownPin, 
                    uint8_t pwmUpPin, 
                    uint8_t directionDownPin1, 
                    uint8_t directionDownPin2,
                    uint8_t directionUpPin1,
                    uint8_t directionUpPin2
                    )
{
	_downMotorThrower = new DCMotor(pwmDownPin, directionDownPin1, directionDownPin2);
	_upMotorThrower = new DCMotor(pwmUpPin, directionUpPin1, directionUpPin2);
}

DualMotorDriver::~DualMotorDriver()
{
	stopUp();
	stopDown();
}

void DualMotorDriver::setSpeedUp(uint8_t speed){
    _upMotorThrower->setSpeed(speed);
}

void DualMotorDriver::setSpeedDown(uint8_t speed){
    _downMotorThrower->setSpeed(speed);
}
    void setSpeedUp(int speed);
    void setSpeedDown(int speed);
    void setDirectionUp(bool isForward);
    void setDirectionDown(bool isForward);
    void stopUp();
    void stopDown();