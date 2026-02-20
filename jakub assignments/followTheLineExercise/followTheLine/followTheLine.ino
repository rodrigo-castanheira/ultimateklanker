const int FORWARD_LEFT = 6;
const int FORWARD_RIGHT = 5;
const int BACK_LEFT = 11;
const int BACK_RIGHT = 10;
const int ECHO_PIN = 7;
const int TRIGGER_PIN = 12;
const int SENSOR_LEFT = 2;
const int SENSOR_RIGHT = 3;

int threshold = 480;
int baseSpeed = 200;

volatile long rotationCounterLeft = 0;
volatile long rotationCounterRight = 0;
float errorIntegral = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(FORWARD_LEFT, OUTPUT);
  pinMode(FORWARD_RIGHT, OUTPUT);
  pinMode(BACK_LEFT, OUTPUT);
  pinMode(BACK_RIGHT, OUTPUT);
  pinMode(ECHO_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);
  pinMode(SENSOR_RIGHT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR_LEFT), countLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_RIGHT), countRight, RISING);
  
  delay(2000);
}

void loop() {  
  int s1 = analogRead(A0);
  int s2 = analogRead(A1);
  int s3 = analogRead(A2);
  int s4 = analogRead(A4);
  int s5 = analogRead(A3);
  int s6 = analogRead(A5);
  int s7 = analogRead(A6);
  int s8 = analogRead(A7);

//  Serial.println(s1);
//  Serial.println(s2);
//  Serial.println(s3);
//  Serial.println(s4);
//  Serial.println(s5);
//  Serial.println(s6);
//  Serial.println(s7);
//  Serial.println(s8);
//  Serial.println("__________");
//  delay(2000);
  
  if((s4 > threshold && s5 > threshold) || (s2 > threshold && s3 > threshold && s4 > threshold && s5 > threshold) || (s4 > threshold && s5 > threshold && s6 > threshold && s7 > threshold && s8 > threshold)) {
    balanceMotors(baseSpeed + 55, true);
  }
  else if (s3 > threshold && s4 > threshold) {
    moveMotors(baseSpeed + 20, baseSpeed - 20);
  } 
  else if (s5 > threshold && s6 > threshold) {
    moveMotors(baseSpeed - 20, baseSpeed + 20);
  }
  else if (s2 > threshold){
    moveMotors(baseSpeed + 40, 0); 
  }
  else if (s7 > threshold){
    moveMotors(0, baseSpeed + 40); 
  }
  else if (s2 > threshold && s1 > threshold){
    moveMotors(baseSpeed + 55, 0); 
  }
  else if (s7 > threshold && s8 > threshold){
    moveMotors(0, baseSpeed + 55); 
  }
}

void countLeft(){
  rotationCounterLeft++;
}

void countRight(){
  rotationCounterRight++;  
}

void balanceMotors(int baseSpeed, bool forward) {
  const float PROPORTIONAL_GAIN = 15;
  const float INTEGRAL_GAIN = 0.05;
  const float DERIVATIVE_GAIN = 0.3;
  
  const float MAX_INTEGRAL = 150.0;

  int lastDifference = 0;

  int difference = rotationCounterLeft - rotationCounterRight;

  errorIntegral += difference;
  errorIntegral = constrain(errorIntegral, -MAX_INTEGRAL, MAX_INTEGRAL);
  
  int derivative = difference - lastDifference;
  lastDifference = difference;

  float correction = (PROPORTIONAL_GAIN * difference) + (INTEGRAL_GAIN * errorIntegral) + (DERIVATIVE_GAIN * derivative);

  int speedLeft = baseSpeed - correction;
  int speedRight = baseSpeed + correction;

  speedLeft = constrain(speedLeft, 0, 255);
  speedRight = constrain(speedRight, 0, 255);

  if (forward) {
    analogWrite(FORWARD_LEFT, speedLeft);
    analogWrite(BACK_LEFT, 0);
    analogWrite(FORWARD_RIGHT, speedRight);
    analogWrite(BACK_RIGHT, 0);
  } else {
    analogWrite(FORWARD_LEFT, 0);
    analogWrite(BACK_LEFT, speedLeft);
    analogWrite(FORWARD_RIGHT, 0);
    analogWrite(BACK_RIGHT, speedRight);
  }
  
  delay(10); 
}

void moveMotors(int left, int right) {
  analogWrite(FORWARD_LEFT, constrain(left, 0, 255));
  analogWrite(BACK_LEFT, 0);
  analogWrite(FORWARD_RIGHT, constrain(right, 0, 255));
  analogWrite(BACK_RIGHT, 0);
}
