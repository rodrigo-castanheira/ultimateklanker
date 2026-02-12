/*
 * Arduino Smart Car - INFINITE FORWARD RUNNER
 * - Default behavior: Drive Forward continuously
 * - If blocked: Performs "Smart Avoidance" (Box maneuver) then resumes forward
 * - "Corner Cut" Fixes included (Back up + Long drive)
 */

// --- HARDWARE PINS ---
const int LEFT_FORWARD   = 11;
const int LEFT_BACKWARD  = 10;
const int RIGHT_BACKWARD = 3;
const int RIGHT_FORWARD  = 9; 

// Ultrasonic Sensor
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// --- CONFIGURATION ---
const int SPEED_L_FW   = 200; 
const int SPEED_R_FW   = 190;
const int SPEED_L_BACK = 195;
const int SPEED_R_BACK = 190; 

const int OBSTACLE_DIST_CM = 15;     // Trigger avoidance if closer than 20cm

// --- AVOIDANCE TUNING ---
const int SCAN_TURN_TIME    = 400;   // Scan turn duration
const int AVOID_TURN_90     = 500;   // Tuned for 90 degrees (Lowered to prevent over-turn)
const int AVOID_BACKUP_TIME = 350;   // Safety backup

// Drive times for the "Box" maneuver
const int AVOID_DRIVE_WIDTH  = 1000; // Drive AWAY from the line
const int AVOID_DRIVE_LENGTH = 2500; // Drive PAST the object

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initial startup delay
  delay(2000); 
}

void loop() {
  // 1. Check for Obstacles
  int distance = getDistance();
  
  // 2. Decide Action
  if (distance > 0 && distance < OBSTACLE_DIST_CM) {
    // If blocked, go around
    performSmartAvoidance();
  } 
  else {
    // If clear, JUST GO FORWARD
    moveForward();
  }
}

// ---------------------------------------------------------
// SMART AVOIDANCE LOGIC
// ---------------------------------------------------------

void performSmartAvoidance() {
  stopMotors();
  Serial.println("Obstacle! Init Avoidance...");
  
  // FIX 1: Back up slightly so we don't hit the corner while turning
  moveBackward();
  delay(AVOID_BACKUP_TIME);
  stopMotors();

  // 1. SCAN LEFT
  turnLeft();
  delay(SCAN_TURN_TIME);      
  stopMotors();
  int leftDist = getDistance();
  
  // Return Center
  turnRight();
  delay(SCAN_TURN_TIME);      
  stopMotors();

  // 2. SCAN RIGHT
  turnRight();
  delay(SCAN_TURN_TIME);      
  stopMotors();
  int rightDist = getDistance();
  
  // Return Center
  turnLeft();
  delay(SCAN_TURN_TIME);      
  stopMotors();

  // 3. DECIDE AND MOVE
  if (leftDist > rightDist) {
    Serial.println("Going Left...");
    avoidLeft();
  } else {
    Serial.println("Going Right...");
    avoidRight();
  }
  
  // No need to fix timers anymore, just return to loop() which resumes forward motion
  Serial.println("Resuming forward drive.");
}

// MANEUVER: Go around via the RIGHT side
void avoidRight() {
  // 1. Turn Right 90
  turnRight();
  delay(AVOID_TURN_90);
  
  // 2. Drive AWAY from line (Width)
  moveForward();
  delay(AVOID_DRIVE_WIDTH); 
  
  // 3. Turn Left 90 (Parallel)
  turnLeft();
  delay(AVOID_TURN_90);
  
  // 4. Drive PAST obstacle (Length)
  moveForward();
  delay(AVOID_DRIVE_LENGTH); 
  
  // 5. Turn Left 90 (Face back to line)
  turnLeft();
  delay(AVOID_TURN_90);
  
  // 6. Drive BACK to line (Width)
  moveForward();
  delay(AVOID_DRIVE_WIDTH);
  
  // 7. Turn Right 90 (Face original direction)
  turnRight();
  delay(AVOID_TURN_90);
  
  stopMotors();
  delay(500); 
}

// MANEUVER: Go around via the LEFT side
void avoidLeft() {
  // 1. Turn Left 90
  turnLeft();
  delay(AVOID_TURN_90);
  
  // 2. Drive AWAY from line (Width)
  moveForward();
  delay(AVOID_DRIVE_WIDTH);
  
  // 3. Turn Right 90 (Parallel)
  turnRight();
  delay(AVOID_TURN_90);
  
  // 4. Drive PAST obstacle (Length)
  moveForward();
  delay(AVOID_DRIVE_LENGTH); 
  
  // 5. Turn Right 90 (Face back to line)
  turnRight();
  delay(AVOID_TURN_90);
  
  // 6. Drive BACK to line (Width)
  moveForward();
  delay(AVOID_DRIVE_WIDTH);
  
  // 7. Turn Left 90 (Face original direction)
  turnLeft();
  delay(AVOID_TURN_90);
  
  stopMotors();
  delay(500);
}

// ---------------------------------------------------------
// MOTOR FUNCTIONS
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
  // Left Fwd, Right Back
  analogWrite(LEFT_FORWARD, SPEED_L_FW);
  digitalWrite(LEFT_BACKWARD, LOW);
  
  digitalWrite(RIGHT_FORWARD, LOW);
  analogWrite(RIGHT_BACKWARD, SPEED_R_BACK);
}

void turnLeft() {
  // Left Back, Right Fwd
  digitalWrite(LEFT_FORWARD, LOW);
  analogWrite(LEFT_BACKWARD, SPEED_L_BACK);
  
  analogWrite(RIGHT_FORWARD, SPEED_R_FW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void stopMotors() {
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

// ---------------------------------------------------------
// SENSOR FUNCTION
// ---------------------------------------------------------

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  if (duration == 0) return 999; 
  return duration * 0.034 / 2;
}
