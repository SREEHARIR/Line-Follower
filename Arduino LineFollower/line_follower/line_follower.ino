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

