const int LedRed = 8;
const int FastbuttonPin = 4;
const int SlowbuttonPin = 2;

int fastbuttonState = 0;
int lastfastbuttonState = 0;
int slowbuttonState = 0;
int lastslowbuttonState = 0;

bool isFastMode = false;
bool isSlowMode = false;

void setup() {
  pinMode(LedRed, OUTPUT);
  pinMode(FastbuttonPin, INPUT);
  pinMode(SlowbuttonPin, INPUT);
}

void loop() {
  fastbuttonState = digitalRead(FastbuttonPin);
  slowbuttonState = digitalRead(SlowbuttonPin);

  if (fastbuttonState == HIGH && lastfastbuttonState == LOW) {
    isFastMode = !isFastMode;
    if (isFastMode == true) {
      isSlowMode = false;
    }
    delay(50);
  }


  if (slowbuttonState == HIGH && lastslowbuttonState == LOW) {

    isSlowMode = !isSlowMode;
    if (isSlowMode == true) {
      isFastMode = false;
    }
    delay(50);
  }

  lastfastbuttonState = fastbuttonState;
  lastslowbuttonState = slowbuttonState;

  if (isFastMode == true) {
    digitalWrite(LedRed, LOW);
    delay(500); 
    digitalWrite(LedRed, HIGH);
    delay(500);
  } else if (isSlowMode == true) {
    digitalWrite(LedRed, LOW);
    delay(2000); 
    digitalWrite(LedRed, HIGH);
    delay(2000);
  } else {
    digitalWrite(LedRed, LOW);
    delay(1000);
    digitalWrite(LedRed, HIGH);
    delay(1000);
  }
}
