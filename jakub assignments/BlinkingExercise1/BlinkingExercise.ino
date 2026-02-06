const int LedRed = 8;
const int FastButtonPin = 2;
const int SlowButtonPin = 4;

int fastButtonState = 0;
int recentFastButtonState = 0;
int slowButtonState = 0;
int recentSlowButtonState = 0;

bool isFastMode = false;
bool isSlowMode = false;

void setup() {
  pinMode(LedRed, OUTPUT);
  pinMode(FastButtonPin, INPUT);
  pinMode(SlowButtonPin, INPUT);
}

void loop() {
  fastButtonState = digitalRead(FastButtonPin);
  slowButtonState = digitalRead(SlowButtonPin);

  if(fastButtonState == HIGH && recentFastButtonState == LOW) {
      isFastMode = !isFastMode;
      if(isFastMode == true) {
          isSlowMode = false;
        }
      delay(50);
    }

  if(slowButtonState == HIGH && recentSlowButtonState == LOW) {
    isSlowMode = !isSlowMode;
    if(isSlowMode == true) {
        isFastMode = false;
      }
    delay(50);
  }

  recentFastButtonState = fastButtonState;
  recentSlowButtonState = slowButtonState;

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
