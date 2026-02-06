const int ledRed = 8;
const int buttonPin = 4;

int buttonState = 0;
int lastbuttonState = 0;
bool isFastMode = false;

void setup() {
  pinMode(ledRed, OUTPUT);

  pinMode(buttonPin, INPUT);

}

void loop() {
buttonState = digitalRead(buttonPin);

if ( buttonState == HIGH && lastbuttonState == LOW ){
  isFastMode = !isFastMode;
}

lastbuttonState = buttonState;

if (isFastMode == true){
  digitalWrite(ledRed, LOW);
  delay(500);
  digitalWrite(ledRed, HIGH);
  delay(500);
} else {
  digitalWrite(ledRed, LOW);
  delay(1000);
  digitalWrite(ledRed, HIGH);
  delay(1000);
}

}
