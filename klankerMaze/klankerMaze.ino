/*
 * Arduino Maze Solver - FINAL COMPLETE VERSION
 * - Logic: Check Right -> Check Left -> Decide (Right Priority).
 * - Safety: Stall Protection (Encoders on Pins 2 & 3).
 * - Pins: 6, 11, 10, 5.
 */

// --- HARDWARE PINS ---
const int LEFT_FORWARD   = 6;
const int LEFT_BACKWARD  = 11;
const int RIGHT_BACKWARD = 10;
const int RIGHT_FORWARD  = 5; 

// Ultrasonic Sensor
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// Encoders (MUST be Pins 2 & 3 for Interrupts)
const int ENC_PIN_1 = 2;
const int ENC_PIN_2 = 3;

// --- CONFIGURATION ---
const int SPEED_L_FW   = 200; 
const int SPEED_R_FW   = 185;
const int SPEED_L_BACK = 195;
const int SPEED_R_BACK = 190; 

const int WALL_DISTANCE_CM = 10;     

// --- TUNING CONSTANTS ---
const int SCAN_TURN_TIME    = 475;   
const int TURN_90_TIME      = 475;   
const int TURN_180_TIME     = 1100;  
const int BACKUP_TIME       = 400;   

// --- STALL VARIABLES ---
volatile unsigned long wheelClicks = 0; 
unsigned long lastStallCheckTime = 0;
unsigned long lastClickCount = 0;
const int STALL_CHECK_INTERVAL = 500; // Check every 500ms

void setup() {
  Serial.begin(9600);

  // Motor Pins
  pinMode(LEFT_FORWARD, OUTPUT); pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT); pinMode(RIGHT_FORWARD, OUTPUT);

  // Sensor Pins
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);

  // Encoder Pins
  pinMode(ENC_PIN_1, INPUT_PULLUP);
  pinMode(ENC_PIN_2, INPUT_PULLUP);

  // Attach Interrupts
  attachInterrupt(digitalPinToInterrupt(ENC_PIN_1), countMovement, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN_2), countMovement, CHANGE);

  delay(2000); 

  // RESET TIMERS (Fixes "Stuck at Start" bug)
  wheelClicks = 0;
  lastClickCount = 0;
  lastStallCheckTime = millis(); 
}

void loop() {
  // 1. Scan Forward
  int distance = getStableDistance();
  
  // 2. Maze Logic
  if (distance > 0 && distance < WALL_DISTANCE_CM) {
    solveIntersection();
    // Reset stall check so we don't trigger it immediately after stopping
    resetStallCheck(); 
  } 
  else {
    moveForward();
    // 3. ONLY Check for Stalls while moving forward
    checkIfStuck();
  }
}

// ---------------------------------------------------------
// STALL PROTECTION LOGIC
// ---------------------------------------------------------

void countMovement() {
  wheelClicks++;
}

void checkIfStuck() {
  if (millis() - lastStallCheckTime > STALL_CHECK_INTERVAL) {
    
    // If less than 5 clicks in 500ms, assume stuck
    if (wheelClicks - lastClickCount < 5) {
      handleStuckRobot();
    }
    
    lastClickCount = wheelClicks;
    lastStallCheckTime = millis();
  }
}

void handleStuckRobot() {
  stopMotors();
  Serial.println("STUCK! Wheels not turning. Rescuing...");
  
  // 1. Back up
  moveBackward();
  delay(600); 
  stopMotors();
  delay(200);
  
  // 2. Twist Left to free wheels
  turnLeft();
  delay(400); 
  stopMotors();
  delay(200);
  
  resetStallCheck();
}

void resetStallCheck() {
  wheelClicks = 0;
  lastClickCount = 0;
  lastStallCheckTime = millis();
}

// ---------------------------------------------------------
// MAZE DECISION LOGIC (CHECK BOTH SIDES)
// ---------------------------------------------------------

void solveIntersection() {
  stopMotors();
  delay(100);

  // False Alarm Check
  if (getStableDistance() > WALL_DISTANCE_CM + 5) {
    return; 
  }

  Serial.println("Wall! Backing up...");
  moveBackward();
  delay(BACKUP_TIME);
  stopMotors();
  delay(200);

  // --- STEP 1: SCAN RIGHT ---
  Serial.println("Scanning Right...");
  turnRight();
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(300); 
  int rightDist = getStableDistance();
  
  turnLeft(); // Return to Center
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(200);

  // --- STEP 2: SCAN LEFT --- 
  Serial.println("Scanning Left...");
  turnLeft();
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(300);
  int leftDist = getStableDistance();
  
  turnRight(); // Return to Center
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(200);

  // --- STEP 3: DECIDE (Right Priority) ---
  if (rightDist > 20) {
    Serial.println("Go Right");
    turnRight();
    delay(TURN_90_TIME);
  }
  else if (leftDist > 20) {
    Serial.println("Go Left");
    turnLeft();
    delay(TURN_90_TIME);
  }
  else {
    Serial.println("Dead End -> U-Turn");
    turnLeft(); 
    delay(TURN_180_TIME); 
  }
  
  stopMotors();
  delay(500); 
}

// ---------------------------------------------------------
// MOTOR & SENSOR FUNCTIONS
// ---------------------------------------------------------

void moveForward() {
  analogWrite(LEFT_FORWARD, SPEED_L_FW);
  digitalWrite(LEFT_BACKWARD, LOW);
  analogWrite(RIGHT_FORWARD, SPEED_R_FW); 
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void moveBackward() {
  digitalWrite(LEFT_FORWARD, LOW);
  analogWrite(LEFT_BACKWARD, SPEED_L_BACK);
  digitalWrite(RIGHT_FORWARD, LOW);
  analogWrite(RIGHT_BACKWARD, SPEED_R_BACK);
}

void turnRight() {
  analogWrite(LEFT_FORWARD, SPEED_L_FW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  analogWrite(RIGHT_BACKWARD, SPEED_R_BACK);
}

void turnLeft() {
  digitalWrite(LEFT_FORWARD, LOW);
  analogWrite(LEFT_BACKWARD, SPEED_L_BACK);
  analogWrite(RIGHT_FORWARD, SPEED_R_FW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void stopMotors() {
  digitalWrite(LEFT_FORWARD, LOW); digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW); digitalWrite(RIGHT_BACKWARD, LOW);
}

int getStableDistance() {
  int r1 = getSingleReading(); delay(10);
  int r2 = getSingleReading(); delay(10);
  int r3 = getSingleReading();
  if ((r1 <= r2 && r2 <= r3) || (r3 <= r2 && r2 <= r1)) return r2;
  if ((r2 <= r1 && r1 <= r3) || (r3 <= r1 && r1 <= r2)) return r1;
  return r3;
}

int getSingleReading() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}
