/*
    Name:       ballThrower.ino
    Created:	7/11/2021 10:35:07 PM
    Author:     Daler Rahimov
*/

// User Types 
#include <Thrower.h>
#include <DCMotor.h>

#define BOUD_RATE 115200

// Motors 
// motor one
const double SHOTPOWER_PRESENTILE = 0.11; // Power of all shots 

const uint8_t PWM_UP_MOTOR = 3;
const uint8_t DIR_UP_MOTOR1 = A0;
const uint8_t DIR_UP_MOTOR2 = A1;
// motor two 
const uint8_t PWM_DOWN_MOTOR = 4;
const uint8_t DIR_DOWN_MOTOR1 = A4;
const uint8_t DIR_DOWN_MOTOR2 = A5;
// Thrower 
Thrower *thrower;

// Computer cmds defines 
#define CMD_LENTH	4
#define CMD_SOH		0x01 
#define CMD_ETX		0x03
#define CMD_CAHR_ID	1			//command char ID e.g. 'SOH|m2|ETX` id of 'm' 
#define CMD_NUM_ID	2			//command's num ID e.g. 'SOH|m2|ETX` id of '2'

// Result code defines 
// NOTE: Do nat change the current order clients depend on it. Add new ones at the end 
const byte OK =				 0;
const byte ERROR =			 1;
const byte NO_SUCK_SHOT =	 2;
const byte CMD_INC_FORM =	 3;
const byte PARTIAL_CMD =	 4;
const byte MODE_NOT_SET =	 5;
const byte NO_SUCH_CMD  =	 6;
const byte NO_IMPLEMENT =	 7;

//Version 
const char MAJOR =	'1';
const char MINOR =	'0';
char verC[] = { MAJOR,MINOR,0 };
byte verByte = atoi(verC);

// ballThrower 
String g_serial_buf;
Thrower::Constants nextCmd = Thrower::E_Last;

unsigned long time_now = 0;
int period = 5;// period that is used to check if there is a new commands to execute 
unsigned long time_last_mode_change = 0;

void setup()
{
	Serial.begin(BOUD_RATE);
	Serial.setTimeout(100);
    // Changed frequency on Pin 3
    // pinMode(3, 155);
    // TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
    //setPwmFrequency(PWM_DOWN_MOTOR, 8);
    //setPwmFrequency(PWM_UP_MOTOR, 8);

    // set all the motor control pins to outputs
    pinMode(PWM_UP_MOTOR, OUTPUT);
    pinMode(PWM_DOWN_MOTOR, OUTPUT);
    pinMode(DIR_DOWN_MOTOR1, OUTPUT);
    pinMode(DIR_DOWN_MOTOR2, OUTPUT);
    pinMode(DIR_UP_MOTOR1, OUTPUT);
    pinMode(DIR_UP_MOTOR2, OUTPUT);

	DCMotor upMotor(PWM_UP_MOTOR, DIR_UP_MOTOR1, DIR_UP_MOTOR2);
	DCMotor downMotor(PWM_DOWN_MOTOR, DIR_DOWN_MOTOR1, DIR_DOWN_MOTOR2);

    thrower = new Thrower(upMotor, downMotor);
	thrower->setShotPower(SHOTPOWER_PRESENTILE);
	thrower->setShot(Thrower::FLAT_SHOT);
}


void loop()
{
	checkComputerCmd();
	thrower->playShot();

	// in some time perioud check if new cmd was issued 
	if (millis() > time_now + period) {
		time_now = millis();
		if (nextCmd != Thrower::E_Last) {
			time_last_mode_change = millis();
			thrower->setShot(nextCmd);
			nextCmd = Thrower::E_Last;
		}
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

// ====================================
//        Computer type - Commands  
// ====================================

/*!
	@brief	check if there is new commands sent
	and record it into nexCmd variable 
*/
void checkComputerCmd()
{
	char cmd[CMD_LENTH] = {0}; // all to 0 
	if (Serial.available() > 0) {
		Serial.readBytes(cmd, CMD_LENTH);
	}
	else {
		return;
	}

	// Check format 
	if (cmd[0] != CMD_SOH) {
		Serial.write(CMD_INC_FORM);
		serialFlush();
	}else if (cmd[CMD_LENTH-1] != CMD_ETX) {
		Serial.write(PARTIAL_CMD);
		serialFlush();
	}
	else if (cmd[CMD_CAHR_ID] != 'm' && cmd[CMD_CAHR_ID] != 'v') {
		Serial.write(NO_SUCH_CMD);
		serialFlush();
	}
	else {// there is data and correct format 
		//Version 
		if (cmd[CMD_CAHR_ID] == 'v' || cmd[CMD_CAHR_ID] == 'V') {
			Serial.write(verByte);
		}
		//Commans 
		else if (cmd[CMD_CAHR_ID] == 'm' || cmd[CMD_CAHR_ID] == 'M')
		{
			byte res = doCmd(cmd[CMD_NUM_ID]);
			Serial.write(res);
		}
	}

}

/*!
	@brief	check if issued command exits and save it in nextCmd 
	
	@return	result code 
	@note	result code can go only up to 255
*/
byte doCmd(int cmd)
{
	//check if mode exits in enum of EvoRing 
	bool cmd_in_modes = false;
	for (Thrower::Constants mode = Thrower::E_First; mode < Thrower::E_Last;
		mode = Thrower::Constants(mode + 1)) {
		if (cmd == mode) {
			cmd_in_modes = true;
			break;
		}
	}
	if (!cmd_in_modes) {
		return NO_SUCK_SHOT;
	}
	if (!thrower->isModeImplemented(cmd)) {
		return NO_IMPLEMENT;
	}
	//set mode 
	nextCmd = (Thrower::Constants) cmd;
	return OK;
}

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

// ====================================

//Define User Types below here or use a .h file

// #include <DCMotor.h>

// // motor one
// const uint8_t PWM_MOTOR1 = 3;
// const uint8_t DIR1_MOTOR1 = A0;
// const uint8_t DIR2_MOTOR2 = A1;
// DCMotor* motor1;

// void setup()
// {
//     // Changed frequency on Pin 3
//     //pinMode(3, 155);
//     //TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
//     setPwmFrequency(PWM_MOTOR1, 8);

//     // set all the motor control pins to outputs
//     pinMode(PWM_MOTOR1, OUTPUT);
//     pinMode(DIR1_MOTOR1, OUTPUT);
//     pinMode(DIR2_MOTOR2, OUTPUT);

// 	motor1 = new DCMotor(PWM_MOTOR1, DIR1_MOTOR1, DIR2_MOTOR2);
//     motor1->setSpeed(10);
// }

// void loop()
// {
//     motor1->run();
// }

// void setPwmFrequency(int pin, int divisor) {
//     byte mode;
//     if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
//         switch (divisor) {
//         case 1: mode = 0x01; break;
//         case 8: mode = 0x02; break;
//         case 64: mode = 0x03; break;
//         case 256: mode = 0x04; break;
//         case 1024: mode = 0x05; break;
//         default: return;
//         }
//         if (pin == 5 || pin == 6) {
//             TCCR0B = TCCR0B & 0b11111000 | mode;
//         }
//         else {
//             TCCR1B = TCCR1B & 0b11111000 | mode;
//         }
//     }
//     else if (pin == 3 || pin == 11) {
//         switch (divisor) {
//         case 1: mode = 0x01; break;
//         case 8: mode = 0x02; break;
//         case 32: mode = 0x03; break;
//         case 64: mode = 0x04; break;
//         case 128: mode = 0x05; break;
//         case 256: mode = 0x06; break;
//         case 1024: mode = 0x07; break;
//         default: return;
//         }
//         TCCR2B = TCCR2B & 0b11111000 | mode;
//     }
// }