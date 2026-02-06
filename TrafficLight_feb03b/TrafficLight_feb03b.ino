const int LedRed = 8;
const int LedYellow = 12;
const int LedGreen = 13;
const int ButtonPin = 4;

int buttonState = 0;

void setup() {
  pinMode(LedRed, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(ButtonPin, INPUT);

}

void loop() {
buttonState = digitalRead(ButtonPin);

  
if (buttonState == LOW){
  
  digitalWrite(LedRed, LOW);
  digitalWrite(LedYellow, HIGH);
  digitalWrite(LedGreen, HIGH);
  delay(3000);
  digitalWrite(LedRed, HIGH);
  digitalWrite(LedYellow, HIGH);
  digitalWrite(LedGreen, LOW);
  delay(4000);
  digitalWrite(LedRed, HIGH);
  digitalWrite(LedYellow, LOW);
  digitalWrite(LedGreen, HIGH);
  delay(1000);
} else {
  digitalWrite(LedRed, LOW);
  digitalWrite(LedYellow, HIGH);
  digitalWrite(LedGreen, HIGH);
}

}
