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

void loop() {

  //for(i=0;i<256;i++)
  //{
  //  speedr=i;
  //  speedl=i;
  //  drive();
  //  Serial.println(i);
  //  delay(100);
  //}


  //  for (i = 0; i < 8; i++)
  //  {
  //    s[i] = digitalRead(sensor[i]);
  //    Serial.print(i);
  //    Serial.print(" = ");
  //    Serial.println(s[i]);
  //  }
  //  for(i=6;i<8;i++)
  //  {
  //  s[i]=analogRead(sensor[i]);
  //    if(s[i]>300)
  //    s[i]=1;
  //    else
  //    s[i]=0;
  //    Serial.print(i);
  //    Serial.print(" = ");
  //    Serial.println(s[i]);
  //  }
  //  Serial.println();
  //  delay(1000);
  //  result = 0;
  //  for (i = 0; i < 8; i++)
  //  {
  //    result += w[i] * s[i];
  //  }
  //  diff = 2 * (255 * result) / 7;
  //  speedl = 255;
  //  speedr = 255;
  //  if (diff > 0)
  //  {
  //    speedl -= diff;
  //  }
  //  else
  //  {
  //    speedr += diff;
  //  }
  //  if (speedl < -255)
  //  {
  //    speedl = -255;
  //  }
  //  if (speedr < -255)
  //  {
  //    speedr = -255;
  //  }
  //  drive();
  followPID();
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

int read_line()
{
  unsigned char i, on_line = 0;
  unsigned long avg; // this is for the weighted total, which is long before division
  unsigned int sum; // this is for the denominator which is <= 64000
  static int last_value = 0; // assume initially that the line is left.

  readsens();

  avg = 0;
  sum = 0;

  for (i = 0; i < num_sensor; i++)
  {
    int value = !sensor_values[i];

    // keep track of whether we see the line at all
    if (value == 1)
    {
      on_line = 1;
    }

    // only average in values that are above a noise threshold
    if (1)
    {
      avg += (value) * (i * 100);
      sum += value;
    }
  }

  if (!on_line)
  {
    // If it last read to the left of center, return 0.
    if (last_value < (num_sensor-1)*100/2)
      return 0;

    // If it last read to the right of center, return the max.
    else
      return (num_sensor-1)*100;

  }

  last_value = avg / sum;

  return last_value;
}

void readsens()
{
  int i;
  //unsigned char last_time;
  //unsigned char delta_time;
  //unsigned int time = 0;
  for (i = 0; i < num_sensor; i++)
  {
    sensor_values[i] = 0;
  }

  for (i = 0; i < num_sensor; i++)
  {
    sensor_values[i] = digitalRead(sensor[i]);
  }
  
  //  for(i=6;i<8;i++)
  //  {
  //  sensor_values[i] = digitalRead(sensor[i]);
  //    if(s[i]>300)
  //    s[i]=1;
  //    else
  //    s[i]=0;
  //    Serial.print(i);
  //    Serial.print(" = ");
  //    Serial.println(s[i]);
  //  }
  
}
