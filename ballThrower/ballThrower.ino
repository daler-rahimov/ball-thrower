/*
    Name:       ballThrower.ino
    Created:	7/11/2021 10:35:07 PM
    Author:     Daler Rahimov
*/

// User Types 
#include <Thrower.h>
#include <DCMotor.h>

#define BOUD_RATE 115200

// Motor 
Thrower *thrower;
const double SHOTPOWER_PRESENTILE = 0.07; // Power of all shots 

// Computer cmds defines 
#define CMD_SOH		0x01 
#define CMD_ETX		0x03
const char CMD_PER_SET  = 	's';
const char CMD_PER_GET  =	'g';
const char CMD_PER_VER  = 	'v';
const char CMD_PER_MODE =	'm';
const char CMD_PER_SPIN = 	's';
const char CMD_PER_POWER=	'p';

// Result code defines 
// NOTE: Do nat change the current order clients depend on it. Add new ones at the end 
const byte OK =				  0;
const byte ERROR =			 -1;
const byte NO_SUCK_SHOT =	 -2;
const byte CMD_INC_FORM =	 -3;
const byte PARTIAL_CMD =	 -4;
const byte MODE_NOT_SET =	 -5;
const byte NO_SUCH_CMD  =	 -6;
const byte NO_IMPLEMENT =	 -7;
const byte CMD_INC_VALUE=	 -8;

//Version 
const char MAJOR =	'1';
const char MINOR =	'0';
const char PATCH =	'0';
char verC[] = { MAJOR, MINOR, PATCH };
byte verByte = atoi(verC);

// ballThrower 
Thrower::Constants nextMode = Thrower::E_Last;

void setup()
{
	Serial.begin(BOUD_RATE);
	Serial.setTimeout(100);
    // Changed frequency 
    // setPwmFrequency(PWM_DOWN_MOTOR, 8);
    // setPwmFrequency(PWM_UP_MOTOR, 8);

	// Motors 
	
	//************************************
	// DCMotorDROKL298
	//************************************
	// // motor one
	// const uint8_t PWM_UP_MOTOR = 3;
	// const uint8_t DIR_UP_MOTOR1 = A0;
	// const uint8_t DIR_UP_MOTOR2 = A1;
	// // motor two 
	// const uint8_t PWM_DOWN_MOTOR = 4;
	// const uint8_t DIR_DOWN_MOTOR1 = A4;
	// const uint8_t DIR_DOWN_MOTOR2 = A5;
    // // set all the motor control pins to outputs
    // pinMode(PWM_UP_MOTOR, OUTPUT);
    // pinMode(PWM_DOWN_MOTOR, OUTPUT);
    // pinMode(DIR_DOWN_MOTOR1, OUTPUT);
    // pinMode(DIR_DOWN_MOTOR2, OUTPUT);
    // pinMode(DIR_UP_MOTOR1, OUTPUT);
    // pinMode(DIR_UP_MOTOR2, OUTPUT);
	// DCMotorDROKL298 upMotor(PWM_UP_MOTOR, DIR_UP_MOTOR1, DIR_UP_MOTOR2);
	// DCMotorDROKL298 downMotor(PWM_DOWN_MOTOR, DIR_DOWN_MOTOR1, DIR_DOWN_MOTOR2);

	//***********************************
	// DCMotorCytron
	//***********************************
	// motor one
	const uint8_t PWM_UP_MOTOR = 3;
	const uint8_t DIR_UP_MOTOR = A0;
	// motor two 
	const uint8_t PWM_DOWN_MOTOR = 4;
	const uint8_t DIR_DOWN_MOTOR = A1;
    // set all the motor control pins to outputs
    pinMode(PWM_UP_MOTOR, OUTPUT);
    pinMode(PWM_DOWN_MOTOR, OUTPUT);
    pinMode(DIR_DOWN_MOTOR, OUTPUT);
    pinMode(DIR_UP_MOTOR, OUTPUT);
	DCMotorCytron upMotor(PWM_UP_MOTOR, DIR_UP_MOTOR);
	DCMotorCytron downMotor(PWM_DOWN_MOTOR, DIR_DOWN_MOTOR);


    thrower = new Thrower(upMotor, downMotor);
	thrower->setShotPower(SHOTPOWER_PRESENTILE);
	thrower->setShot(Thrower::FLAT_SHOT);
}


const byte CMD_LENTH = 5; // 
char cmd[CMD_LENTH] = {NULL}; // all to 0 
char nextCmd[CMD_LENTH-2] = {NULL};

void loop()
{
	thrower->playShot();
	checkComputerCmd();
	doNextCmd();
}

/*!
	@brief	check if there is new commands record it into nexCmd variable 
*/
void checkComputerCmd()
{
	if (Serial.available() > 0) {
		
		Serial.readBytes(cmd, CMD_LENTH);
		if (cmd[0] != CMD_SOH ){
			Serial.write(CMD_INC_FORM);
		} else if (cmd[CMD_LENTH-1] != CMD_ETX){
			Serial.write(PARTIAL_CMD);
		} else {// there is data and correct format
			nextCmd[0] = cmd[1]; 
			nextCmd[1] = cmd[2];
			nextCmd[2] = cmd[3]; 
		}
		serialFlush();
	}
}

void doNextCmd(){

	if (nextCmd[0] == NULL) 
		return;
	// Serial.write(nextCmd[0]);
	char firt_c = nextCmd[0];
	if (firt_c != CMD_PER_GET && firt_c != CMD_PER_SET && firt_c != CMD_PER_VER){
		Serial.write(NO_SUCH_CMD);
	} 
	// Version 
	else if (firt_c == 'v'){
		Serial.write(verByte);
	}
	// Set cmd 
	else if (firt_c == 's'){
		byte res = setCmd();
		Serial.write(res);
	}
	// Get 
	else if (firt_c == 'g'){
		byte res = getCmd();
		Serial.write(res);
	}

	clearNextCmd();
}

/*!
	@brief	Preform all "GET" CMDs   
	
	@return	result code  
	@note	result code can go only up to 255
*/
byte getCmd(){
	if (nextCmd[0] != CMD_PER_GET){// senety check 
		return NO_SUCK_SHOT; 
	} else if (nextCmd[1] == CMD_PER_POWER) { // Get shot power
		double shotPowerPresentile = thrower->getShotPower();
		byte res = (byte)(shotPowerPresentile * 100);
		return res;
	} else if (nextCmd[1] == CMD_PER_SPIN) { // Get spin 
		double spinPresentile = thrower->getSpin();
		byte res = (byte)(spinPresentile * 100);
		return res;
	} else {
		return NO_SUCH_CMD;
	}
}

/*!
	@brief	Preform all "SET" CMDs   
	
	@return	result code 
	@note	result code can go only up to 255
*/
byte setCmd()
{
	if (nextCmd[0] != CMD_PER_SET){// senety check 
		return NO_SUCK_SHOT; 
	} else if (nextCmd[1] == CMD_PER_MODE){// Set mode  
		char modeCmd = nextCmd[2];
		//check if mode exits in enum of Thrower 
		bool cmd_in_modes = false;
		for (Thrower::Constants mode = Thrower::E_First; mode < Thrower::E_Last;
			mode = Thrower::Constants(mode + 1)) {
			if (modeCmd== mode) {
				cmd_in_modes = true;
				break;
			}
		}
		if (!cmd_in_modes) {
			return NO_SUCK_SHOT;
		}
		if (!thrower->isModeImplemented(modeCmd)) {
			return NO_IMPLEMENT;
		}
		//set mode 
		nextMode = (Thrower::Constants) modeCmd;
		thrower->setShot(nextMode);
		return OK;
	} else if (nextCmd[1] == CMD_PER_POWER){// Set shot power  
		double powerPerentile = double(nextCmd[2])/100.00;
		if (powerPerentile < 0 || powerPerentile > 1 ){
			return CMD_INC_VALUE;
		}else{
			thrower->setShotPower(powerPerentile);
			return OK;
		}
	} else if (nextCmd[1] == CMD_PER_SPIN){// Set spin
		double spinPerentile = double(nextCmd[2])/100.00;
		if (spinPerentile < -1 || spinPerentile > 1 ){
			return CMD_INC_VALUE;
		} else {
			thrower->setSpin(spinPerentile);
			return OK;
		}
	} else {
		return NO_SUCH_CMD;
	}
}

/*!
	@brief	flush all data from serial 
*/
void serialFlush() {
	while (Serial.available() > 0) {
		char t = Serial.read();
	}
}

void clearNextCmd(){
	for (int i=0; i<CMD_LENTH-2; i++){
		nextCmd[i] = 0;
	}
}

/*!
	@brief	Set PWM frequency 
*/
void setPwmFrequency(int pin, int divisor) {
    byte mode;
    if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
        switch (divisor) {
        case 1: mode = 0x01; break;
        case 8: mode = 0x02; break;
        case 64: mode = 0x03; break;
        case 256: mode = 0x04; break;
        case 1024: mode = 0x05; break;
        default: return;
        }
        if (pin == 5 || pin == 6) {
            TCCR0B = TCCR0B & 0b11111000 | mode;
        }
        else {
            TCCR1B = TCCR1B & 0b11111000 | mode;
        }
    }
    else if (pin == 3 || pin == 11) {
        switch (divisor) {
        case 1: mode = 0x01; break;
        case 8: mode = 0x02; break;
        case 32: mode = 0x03; break;
        case 64: mode = 0x04; break;
        case 128: mode = 0x05; break;
        case 256: mode = 0x06; break;
        case 1024: mode = 0x07; break;
        default: return;
        }
        TCCR2B = TCCR2B & 0b11111000 | mode;
    }
}
