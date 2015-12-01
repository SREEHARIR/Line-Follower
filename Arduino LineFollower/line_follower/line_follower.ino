/* Pins */
int sensor[8] = {13, 12, 11, 10, 9, 8, 1, 0}; /*left to right*/
int el = 6;/*looking from back*/
int er = 5;
int mlp = 2;
int mln = 3;
int mrp = 4;
int mrn = 7;

/* Variables */
int i, j;
int num_sensor = 8;
int sensor_values[8];
int w[8] = { -4, -3, -2, -1, 1, 2, 3, 4};
int result;
int speedl;
int speedr;
int diff;

/*Functions*/
void drive();

void setup() {
  for (i = 0; i < 8; i++)
  {
    pinMode(sensor[i], INPUT);
  }
  pinMode(mlp, OUTPUT);
  pinMode(mln, OUTPUT);
  pinMode(mrp, OUTPUT);
  pinMode(mrn, OUTPUT);
  pinMode(el, OUTPUT);
  pinMode(er, OUTPUT);

  //Serial.begin(9600);
}

void motors()
{
  if (speedl > 255.0/2)
  {
    digitalWrite(mlp, 1);
    digitalWrite(mln, 0);
    analogWrite(el, speedl);
  }
  else
  {
    digitalWrite(mlp, 0);
    digitalWrite(mln, 1);
    speedl = (255/2) * speedl;
    analogWrite(el, speedl);
  }
  if (speedr > 255.0/2)
  {
    digitalWrite(mrp, 1);
    digitalWrite(mrn, 0);
    analogWrite(er, speedr);

  }
  else
  {
    digitalWrite(mrp, 0);
    digitalWrite(mrn, 1);
    speedr = (255/2) + speedr;
    analogWrite(er, speedr);
  }

}
/*
--------------------------------------------------------------------------------------------------------------------
*/
float KP = 1.0 ;
float KI = 0.00002 ;
float KD = 16 / 1.0;
int   integral  = 0;
int   last_proportional = 0;

void followPID()
{
  int pos = read_line();

  int center = (( (num_sensor - 1) * 100) / 2);

  int proportional = pos - center;

  int derivative = proportional - last_proportional;

  int power_difference = proportional * KP + integral * KI + derivative * KD;
  last_proportional    = proportional;
  integral  += proportional;


  const int max = 255;
  const int max_diffrence = 500;
  const int factor_diffrence = 2;

  if (power_difference >= max)
    power_difference = max;
  if (power_difference < -max)
    power_difference = -max;


  // if diffrence is too much robot skids
  if (power_difference > 0)
  {
     speedl  = max;
     speedr = max - power_difference;
  }
  else if (power_difference < 0)
  {
    speedl  = max + power_difference;
    speedr = max;
  }

  //
  //    if(leftMotorSpeed - rightMotorSpeed > max_diffrence)
  //    {
  //        leftMotorSpeed -= (leftMotorSpeed - rightMotorSpeed)/factor_diffrence;
  //    }
  //    else if(rightMotorSpeed - leftMotorSpeed > max_diffrence)
  //    {
  //        rightMotorSpeed -= (rightMotorSpeed - leftMotorSpeed)/factor_diffrence;
  //    }
  motors();

}
