const int FORWARD_LEFT = 6;
const int FORWARD_RIGHT = 5;
const int BACK_LEFT = 11;
const int BACK_RIGHT = 10;
const int ECHO_PIN = 7;
const int TRIGGER_PIN = 12;
const int ENCODER_LEFT = 2;
const int ENCODER_RIGHT = 3;

const int SENSOR_PINS[] = {A0, A1, A2, A4, A3, A5, A6, A7};
int s[8];

const int threshold = 480;
const int sensorStopValue = 900;
int baseSpeed = 215;

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
  pinMode(ENCODER_LEFT, INPUT_PULLUP);
  pinMode(ENCODER_RIGHT, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), countLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), countRight, RISING);
}

void loop() {  
  for (int i = 0; i < 8; i++) {
    s[i] = analogRead(SENSOR_PINS[i]);
  }

//  Serial.println(s[0]);
//  Serial.println(s[1]);
//  Serial.println(s[2]);
//  Serial.println(s[3]);
//  Serial.println(s[4]);
//  Serial.println(s[5]);
//  Serial.println(s[6]);
//  Serial.println(s[7]);
//  Serial.println("__________");
//  delay(2000);

    if(s[0] > sensorStopValue && s[1] > sensorStopValue && s[2] > sensorStopValue && s[3] > sensorStopValue && 
       s[4] > sensorStopValue && s[5] > sensorStopValue && s[6] > sensorStopValue && s[7] > sensorStopValue){
      stopMotors();
    }
    else if((s[3] > threshold && s[4] > threshold) || 
       (s[1] > threshold && s[2] > threshold && s[3] > threshold && s[4] > threshold) || 
       (s[3] > threshold && s[4] > threshold && s[5] > threshold && s[6] > threshold && s[7] > threshold)) {
      balanceMotors(baseSpeed + 55, true);
    }
    else if (s[2] > threshold && s[3] > threshold) {
      moveMotors(baseSpeed + 20, baseSpeed - 20);
    } 
    else if (s[4] > threshold && s[5] > threshold) {
      moveMotors(baseSpeed - 20, baseSpeed + 20);
    }
    else if (s[1] > threshold){
      moveMotors(baseSpeed + 40, 0); 
    }
    else if (s[6] > threshold){
      moveMotors(0, baseSpeed + 40); 
    }
    else if (s[1] > threshold && s[0] > threshold){
      moveMotors(baseSpeed + 55, 0); 
    }
    else if (s[6] > threshold && s[7] > threshold){
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

void stopMotors(){
  analogWrite(BACK_LEFT, 0);
  analogWrite(BACK_RIGHT, 0);
  analogWrite(FORWARD_LEFT, 0);
  analogWrite(FORWARD_RIGHT, 0);
}
