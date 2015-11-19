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
