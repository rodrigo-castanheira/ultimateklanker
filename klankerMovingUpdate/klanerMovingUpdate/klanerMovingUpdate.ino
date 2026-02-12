const int LEFT_FORWARD = 11;
const int LEFT_BACKWARD = 10;
const int RIGHT_BACKWARD = 3;
const int RIGHT_FORWARD = 9;

int oneMeterDelay = 4000;


const int RIGHT_SPEED_FORWARD = 190;
const int LEFT_SPEED_FORWARD = 210;
const int RIGHT_SPEED_BACKWARD = 190;
const int LEFT_SPEED_BACKWARD = 210;

void setup() {

  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);

  delay(5000);
  goForward();
  delay(oneMeterDelay);
  stopMotors();
  goBackward();
  delay(oneMeterDelay);
  stopMotors();
  delay(1500);
  rotateRight();
  delay(750);
  stopMotors();
}

void loop() {

}

void stopMotors() {
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void goForward(){
    digitalWrite(LEFT_FORWARD, HIGH);
    digitalWrite(RIGHT_FORWARD, HIGH);
    analogWrite(LEFT_FORWARD, LEFT_SPEED_BACKWARD);
    analogWrite(RIGHT_FORWARD, RIGHT_SPEED_FORWARD);
  }

void goBackward(){
  
    digitalWrite(LEFT_BACKWARD, HIGH);
    digitalWrite(RIGHT_BACKWARD, HIGH);
    analogWrite(LEFT_BACKWARD, LEFT_SPEED_BACKWARD);
    analogWrite(RIGHT_BACKWARD, RIGHT_SPEED_BACKWARD);
  }

  void rotateRight(){
    digitalWrite(RIGHT_FORWARD, HIGH);
    digitalWrite(LEFT_BACKWARD, HIGH);
    analogWrite(RIGHT_FORWARD, RIGHT_SPEED_FORWARD);
    analogWrite(LEFT_BACKWARD, LEFT_SPEED_BACKWARD);
  }
