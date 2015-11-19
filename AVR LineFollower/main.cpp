#include<avr/io.h>
#include<util/delay.h>

void motors(int,int);
int read_line();
void readsens();
void followPID();
void init_motors();

int sensor_values[8];
// char inputs[8]={'C0','C1','C2','C3','C4','C5','B4','B5'};
int num_sensor = 8;


#define READ(port,pin) (PIN ## port & (1<<pin))

#define leftMotorPWMPin		OCR1B
#define rightMotorPWMPin	OCR1A
void init_motors()
{
	//Configure PWM pins OC1B and OC1A to output mode
//DDRB |= (1<<PIND4) | (1<<PIND5);
	//DDRD = 0xFF;
	DDRB=0x0F;
	//Configure motor direction control pins to output mode
//DDRB |= (1<<PIND6) | (1<<PIND7);
	//Clear OC1A/OC1B on compare match when up-counting and set OC1A/OC1B 
	//on compare match when downcounting (sets PWM to non-inverting mode)
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1);	
	//Selects prescalar value 64
	TCCR1B |= (1<<CS10) | (1<<CS11);
	//Phase Correct PWM mode is selected
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13);
	//Sets TOP value to be 250. Frequency is 500Hz
	ICR1 = 250;
}

void motors(int leftMotorSpeed, int rightMotorSpeed)
{
	if(leftMotorSpeed >= 0)
    {
        PORTB &= 0b00000100;
		leftMotorPWMPin = leftMotorSpeed;
		//PORTB |= 0b00001000;
		//PORTB &= ~(1<<PIND7);
		//PORTB = 0b00000001;
    }
    else
    {
		PORTB |= 0b00001000;
        leftMotorPWMPin = 250 + leftMotorSpeed;
		//PORTB |= 0b00000100;
        //PORTB |= 1<<PIND7;
		//PORTB = 0b00000010;
    }
	if(rightMotorSpeed >= 0)
    {
		PORTB &= 0b00000010;
        rightMotorPWMPin = rightMotorSpeed;
		//PORTB |= 0b00000010;
        //PORTB &= ~(1<<PIND6);
		//PORTB = 0b00000100;
    }
    else
    {
		PORTB |= 0b00000001;
        rightMotorPWMPin = 250 + rightMotorSpeed;
		//PORTB |= 0b00000001;
        //PORTB |= 1<<PIND6;
		//PORTB = 0b00001000;
    }
}

