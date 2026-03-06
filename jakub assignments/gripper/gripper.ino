const int SERVO_PIN = 9;
const int LEFT_FORWARD = 6;
const int RIGHT_FORWARD = 5;
const int LEFT_BACKWARD = 11;
const int RIGHT_BACKWARD = 10;

const int OPEN_CLAWS_MS  = 1950;
const int CLOSE_CLAWS_MS = 1050;
int servoTargetValue = OPEN_CLAWS_MS;
long lastServoStatus = 0;

void setup() {
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);

  delay(1000);
  moveForward();
  delay(1000);
  stopMotors();
  delay(1000);
  servoTargetValue = OPEN_CLAWS_MS;
  moveClaws(800);
  servoTargetValue = CLOSE_CLAWS_MS;
  moveClaws(1000);
  moveForward();
  delay(1000);
  stopMotors();
  delay(1000);
  servoTargetValue = OPEN_CLAWS_MS;
  moveClaws(800);
}

void loop() {
  servoUpdate();
}

void servoUpdate() {
  long now = millis();
  if(now - lastServoStatus >= 20){
    lastServoStatus = now;
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(servoTargetValue);
    digitalWrite(SERVO_PIN, LOW);
  }
}

void moveClaws(long ms) {
  long start = millis();
  while(millis() - start < ms){
    servoUpdate();
  }
}

void stopMotors(){
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void moveForward(){
  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(RIGHT_FORWARD, HIGH);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}
