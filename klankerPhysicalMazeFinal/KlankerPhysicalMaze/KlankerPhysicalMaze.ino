#include <Adafruit_NeoPixel.h>

const int FORWARD_LEFT = 6;
const int FORWARD_RIGHT = 5;
const int BACK_LEFT = 11;
const int BACK_RIGHT = 10;
const int TRIGGER_FRONT = 12;
const int ECHO_FRONT = 13;
const int TRIGGER_RIGHT = 7;
const int ECHO_RIGHT = 8;
const int NEO_PIXEL = 4;

//const int SENSOR_PINS[] = {A0, A1, A2, A4, A3, A5, A6, A7};
//int s[8];

//MOVEMENT
const int BASE_SPEED = 200;
const int PANIC_SPEED = 255;
boolean isMoving = true;
int stage = -1;
bool isPanicking = false;
int lastFrontDistance = 0;

unsigned long startRT = 0;
unsigned long startSRT = 0;
unsigned long startART = 0;
unsigned long startLT = 0;
unsigned long startSLT = 0;
unsigned long startALT = 0;
unsigned long lastDistanceCheckTime = 0;
unsigned long panicStartTime = 0;
unsigned long turnExitTime = 0;

//VALUES TO ADJUST
const long BLINDNESS_DURATION = 1000;
const int TURN_DURATION = 400;
const int STOP_DURATION = 150;
const int DRIVE_PAST_DURATION = 500;
const int START_PANIC_AFTER = 2000;
const int BACKUP_DURATION = 800;

//NEOPIXELS
const int TOTAL_PIXELS = 4;
const int ARR_SIZE = 3;
const int TURN_COLOR[ARR_SIZE] = {50, 105, 0};
const int MOVE_COLOR[ARR_SIZE] = {50, 0, 100};
const int STOP_COLOR[ARR_SIZE] = {0, 100, 0};

Adafruit_NeoPixel pixels(TOTAL_PIXELS, NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(FORWARD_LEFT, OUTPUT);
  pinMode(FORWARD_RIGHT, OUTPUT);
  pinMode(BACK_LEFT, OUTPUT);
  pinMode(BACK_RIGHT, OUTPUT);
  pinMode(TRIGGER_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIGGER_RIGHT, OUTPUT);
  pinMode(ECHO_RIGHT, INPUT);
  pixels.begin();
  pixels.clear();
}

void loop() {
  unsigned long currentTime = millis();
  int getDistanceF = getDistanceFront();
  int getDistanceR = getDistanceRight();

  if (stage == -1 && getDistanceF < 30 && getDistanceF > 0) {
    if (abs(getDistanceF - lastFrontDistance) > 1) {
      lastDistanceCheckTime = currentTime;
      lastFrontDistance = getDistanceF;
    } else {
      if (currentTime - lastDistanceCheckTime >= START_PANIC_AFTER) {
        stage = 5;
        panicStartTime = currentTime;
      }
    }
  }

  if (stage == 5) {
    moveBackward(PANIC_SPEED);
    
    if (currentTime - panicStartTime >= BACKUP_DURATION) {
      stopMotors();
      lastDistanceCheckTime = currentTime;
      stage = -1;
    }
  }

  if (stage == -1) {
    moveForward(BASE_SPEED, true);

    if (currentTime - turnExitTime >= BLINDNESS_DURATION) {
      if (getDistanceR >= 25 || getDistanceR == -1) { 
        startSRT = currentTime;
        stage = 4;
        return;
      }
    }

    if (getDistanceF > 0 && getDistanceF < 10) {
      stopMotors();
      startSRT = currentTime;
      
      if (getDistanceR > 0 && getDistanceR < 15) {
        stage = 10;
      } else {
        stage = 1;
      }
    }
  }

  else if (stage == 4) {
    moveForward(BASE_SPEED, true);
    if (currentTime - startSRT >= DRIVE_PAST_DURATION) {
      stopMotors();
      startSRT = currentTime;
      stage = 1; 
    }
  }

  else if (stage == 1 || stage == 10) {
    if (currentTime - startSRT >= STOP_DURATION) {
      if (stage == 1) {
        startRightTurn(); 
      }
      else {
        startLeftTurn(); 
      }
      
      startRT = currentTime;
      stage = 2;
    }
  }

  else if (stage == 2) {
    if (currentTime - startRT >= TURN_DURATION) {
      stopMotors();
      startART = currentTime;
      stage = 3;
    }
  }
  
  else if (stage == 3) {
    if (currentTime - startART >= STOP_DURATION) {
      turnExitTime = currentTime;
      stage = -1;
    }
  }
}

//ROBOT MOVEMENT

void moveForward(int baseSpeed, boolean isMoving){
  if(isMoving){
    pixels.clear();
    pixels.setPixelColor(3, pixels.Color(MOVE_COLOR[0], MOVE_COLOR[1], MOVE_COLOR[2]));
    pixels.setPixelColor(2, pixels.Color(MOVE_COLOR[0], MOVE_COLOR[1], MOVE_COLOR[2]));
    pixels.show();
    
    const int TARGET_DISTANCE = 8;
    const double ADJUSTMENT_VALUE = 10.0;
    int currentDistanceR = getDistanceRight();
    
    if(currentDistanceR > 0 && currentDistanceR < 20){
      int error = currentDistanceR - TARGET_DISTANCE;
      int correction = error * ADJUSTMENT_VALUE;

      int speedLeft = baseSpeed + correction;
      int speedRight = baseSpeed - correction;

      speedLeft = constrain(speedLeft, 0, 255);
      speedRight = constrain(speedRight, 0, 255);

      analogWrite(FORWARD_LEFT, speedLeft);
      analogWrite(FORWARD_RIGHT, speedRight);
    } else {
      analogWrite(FORWARD_LEFT, baseSpeed);
      analogWrite(FORWARD_RIGHT, baseSpeed);  
    }
  }
}

void moveBackward(int baseSpeed) {
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
  pixels.setPixelColor(1, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
  pixels.show();
  analogWrite(FORWARD_LEFT, 0);
  analogWrite(FORWARD_RIGHT, 0);
  analogWrite(BACK_LEFT, baseSpeed);
  analogWrite(BACK_RIGHT, baseSpeed);
}

void stopMotors(){
  pixels.clear();
  analogWrite(BACK_LEFT, 0);
  analogWrite(BACK_RIGHT, 0);
  analogWrite(FORWARD_LEFT, 0);
  analogWrite(FORWARD_RIGHT, 0);
  pixels.setPixelColor(0, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
  pixels.setPixelColor(1, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
  pixels.show(); 
}

void startRightTurn(){
  pixels.clear();
  analogWrite(BACK_LEFT, 0);
  analogWrite(BACK_RIGHT, 255);
  analogWrite(FORWARD_LEFT, 255);
  analogWrite(FORWARD_RIGHT, 0);
  pixels.setPixelColor(1, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
  pixels.setPixelColor(2, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
  pixels.show();
}

void startLeftTurn(){
  pixels.clear();
  analogWrite(BACK_LEFT, 255);
  analogWrite(BACK_RIGHT, 0);
  analogWrite(FORWARD_LEFT, 0);
  analogWrite(FORWARD_RIGHT, 255);
  pixels.setPixelColor(0, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
  pixels.setPixelColor(3, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
  pixels.show();
}

//READING DISTANCE

int getDistanceFront(){
  digitalWrite(TRIGGER_FRONT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_FRONT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_FRONT, LOW);

  long duration = pulseIn(ECHO_FRONT, HIGH, 10000);

  if (duration == 0) {
    return -1;
  }

  float distance = duration * 0.034 / 2;
  return (int)distance;
}

int getDistanceRight(){
  digitalWrite(TRIGGER_RIGHT, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_RIGHT, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_RIGHT, LOW);

  long duration = pulseIn(ECHO_RIGHT, HIGH, 10000);

  if (duration == 0) {
    return -1;
  }

  float distance = duration * 0.034 / 2;
  return (int)distance;
}

//LEDS LOGIC

void clearColors(){
  pixels.clear();
  pixels.show();
}
