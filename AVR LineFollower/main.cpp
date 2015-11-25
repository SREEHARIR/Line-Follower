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

int read_line()
{
	unsigned char i, on_line = 0;
	unsigned long avg; // this is for the weighted total, which is long before division
	unsigned int sum; // this is for the denominator which is <= 64000
	static int last_value=0; // assume initially that the line is left.

	readsens();

	avg = 0;
	sum = 0;
  
	for(i=0;i<num_sensor;i++) 
	{
		int value = sensor_values[i];
		
		// keep track of whether we see the line at all
		if(value > 200) 
		{
			on_line = 1;
		}
		
		// only average in values that are above a noise threshold
		if(value > 50) 
		{
			avg += (long)(value) * (i * 1000);
			sum += value;
		}
	}

	if(!on_line)
	{
		// If it last read to the left of center, return 0.
		if(last_value < (num_sensor-1)*1000/2)
			return 0;
		
		// If it last read to the right of center, return the max.
		else
			return (num_sensor-1)*1000;

	}

	last_value = avg/sum;

	return last_value;
}

/*
void readsens()
{
	unsigned char _maxValue = 2000;
	unsigned char i;
	unsigned char last_time;
	unsigned char delta_time;
	unsigned int time = 0;
	
	// reset the values
	for(i = 0; i < num_sensor; i++)
		sensor_values[i] = 0;
		
	unsigned char prevTCCR1A = TCCR1A;
	unsigned char prevTCCR1B = TCCR1B;
	TCCR1A |= 0x03;
	TCCR1B = 0x02;		// run timer2 in normal mode at 2.5 MHz
						// this is compatible with OrangutanMotors

	last_time = TCNT1;
	while (time < _maxValue)
	{
		// Keep track of the total time.
		// This implicitly casts the difference to unsigned char, so
		// we don't add negative values.
		delta_time = TCNT2 - last_time;
		time += delta_time;
		last_time += delta_time;
	}

	TCCR1A = prevTCCR1A;
	TCCR1B = prevTCCR1B;
	for(i = 0; i < num_sensor; i++)
		if (!sensor_values[i])
			sensor_values[i] = _maxValue;
			
			
	unsigned char j;
	
	// store current state of various registers
	unsigned char admux = ADMUX;
	unsigned char adcsra = ADCSRA;
	unsigned char ddr = DDRC;
	unsigned char port = PORTC;
	
	int _numSamplesPerSensor=10;

	// wait for any current conversion to finish
	while (ADCSRA & (1 << ADSC))
	
	// reset the values
	for(i = 0; i < num_sensor; i++)
		sensor_values[i] = 0;

	// set all sensor pins to high-Z inputs
//	ANALOG_DDR &= ~_portMask;
//	ANALOG_PORT &= ~_portMask;

	ADCSRA = 0x87;	// configure the ADC
	for (j = 0; j < _numSamplesPerSensor; j++)
	{
		for (i = 0; i < num_sensor; i++)
		{
			ADMUX = (1<<6) | _analogPins[i];// set analog input channel
			ADCSRA |= 1 << ADSC;			// start the conversion
			while (ADCSRA & (1 << ADSC));	// wait for conversion to finish
			sensor_values[i] += ADC;		// add in the conversion result
		}
	}
	
	// get the rounded average of the readings for each sensor
	for (i = 0; i < num_sensor; i++)
		sensor_values[i] = (sensor_values[i] + (_numSamplesPerSensor >> 1)) /
			_numSamplesPerSensor;

	ADMUX = admux;
	ADCSRA = adcsra;
	PORTC = port;
	DDRC = ddr;
}
*/


void readsens()
{
	int i;
	//unsigned char last_time;
	//unsigned char delta_time;
	//unsigned int time = 0;
	for(i = 0; i < num_sensor; i++)
	{
		sensor_values[i] = 0;	
	}
	sensor_values[0]=READ(C,0);
	sensor_values[1]=READ(C,1);
	sensor_values[2]=READ(C,2);
	sensor_values[3]=READ(C,3);
	sensor_values[4]=READ(C,4);
	sensor_values[5]=READ(C,5);
	sensor_values[6]=READ(B,4);
	sensor_values[7]=READ(B,5);
}

float KP = 3 ,KI = 50000 , KD = 16/1;
int   integral  = 0;
int   last_proportional = 0;

void followPID()
{
    int position = read_line();

    int center = (( (num_sensor - 1) * 1000) / 2);

    int proportional = position - center;

    int derivative = proportional - last_proportional;

    int power_difference = proportional / KP + integral / KI + derivative * KD;
    last_proportional    = proportional;
    integral  += proportional;


    const int max = 200;
    const int max_diffrence = 20;
    const int factor_diffrence = 2;

    if(power_difference > max)
        power_difference = max;
    if(power_difference < -max)
        power_difference = -max;

    
    // if diffrence is too much robot skids 

    int leftMotorSpeed  = max;
    int rightMotorSpeed = max-power_difference;

    if(power_difference < 0)
    {
        leftMotorSpeed  = max+power_difference;
        rightMotorSpeed = max;
    }


    if(leftMotorSpeed - rightMotorSpeed > max_diffrence)
    {
        leftMotorSpeed -= (leftMotorSpeed - rightMotorSpeed)/factor_diffrence;
    } 
    else if(rightMotorSpeed - leftMotorSpeed > max_diffrence)
    {
        rightMotorSpeed -= (rightMotorSpeed - leftMotorSpeed)/factor_diffrence;
    }

    motors(leftMotorSpeed,rightMotorSpeed);

}

int main(void)
{
	DDRB=0x0F;
	DDRC=0x00;
	DDRD=0xFF;
	init_motors();
	while(1)
	{
		followPID();
		
	//	for(int i=-250;i<250;i++)
		{	
	//		motors(i,i);
	//		_delay_ms(10);
		}
	
	
	}
}