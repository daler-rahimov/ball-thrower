/*
    Name:       
    Author:     Daler Rahimov 
*/

// Define User Types below here or use a .h file
//
#include <DCMotor.h>

// motor one
const uint8_t PWM_MOTOR1 = 3;
const uint8_t DIR1_MOTOR1 = A0;
const uint8_t DIR2_MOTOR2 = A1;
DCMotorDROKL298* motor1;

void setup()
{
    // Changed frequency on Pin 3
    //pinMode(3, 155);
    //TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
    setPwmFrequency(PWM_MOTOR1, 8);

    // set all the motor control pins to outputs
    pinMode(PWM_MOTOR1, OUTPUT);
    pinMode(DIR1_MOTOR1, OUTPUT);
    pinMode(DIR2_MOTOR2, OUTPUT);

	motor1 = new DCMotorDROKL298(PWM_MOTOR1, DIR1_MOTOR1, DIR2_MOTOR2);
    motor1->setSpeed(40);
}

void loop()
{
    motor1->run();
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