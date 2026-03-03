const int SERVO_PIN = 9;
const int OPEN_US  = 1950;
const int CLOSE_US = 1050;

const int LEFT_FORWARD = 6;
const int RIGHT_FORWARD = 5;
const int LEFT_BACKWARD = 11;
const int RIGHT_BACKWARD = 10;

int servoTargetUs = OPEN_US;
long lastServoFrameMs = 0;

void servoUpdate() {
  long now = millis();
  if (now - lastServoFrameMs >= 20) {
    lastServoFrameMs = now;
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(servoTargetUs);
    digitalWrite(SERVO_PIN, LOW);
  }
}
void runFor(long ms) {
  long start = millis();
  while (millis() - start < ms) {
    servoUpdate(); // keep servo active
  }
}

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
  servoTargetUs = OPEN_US;
  runFor(800);
  servoTargetUs = CLOSE_US;
  runFor(1000);
  moveForward();
  delay(1000);
  stopMotors();
  delay(1000);
  servoTargetUs = OPEN_US;
  runFor(800);
}

void loop() {
  servoUpdate();
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
