const int LED_RED = 8;
const int FAST_BUTTON_PIN = 4;
const int SLOW_BUTTON_PIN = 2;

int fastbuttonState = 0;
int lastfastbuttonState = 0;
int slowbuttonState = 0;
int lastslowbuttonState = 0;

bool isFastMode = false;
bool isSlowMode = false;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(FAST_BUTTON_PIN, INPUT);
  pinMode(SLOW_BUTTON_PIN, INPUT);
}

void loop() {
  fastbuttonState = digitalRead(FAST_BUTTON_PIN);
  slowbuttonState = digitalRead(SLOW_BUTTON_PIN);

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
    digitalWrite(LED_RED, LOW);
    delay(500); 
    digitalWrite(LED_RED, HIGH);
    delay(500);
  } else if (isSlowMode == true) {
    digitalWrite(LED_RED, LOW);
    delay(2000); 
    digitalWrite(LED_RED, HIGH);
    delay(2000);
  } else {
    digitalWrite(LED_RED, LOW);
    delay(1000);
    digitalWrite(LED_RED, HIGH);
    delay(1000);
  }
}
