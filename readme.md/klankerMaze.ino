/*
 * Arduino Maze Solver - 450ms BACKUP
 * - Stop Distance: 10cm
 * - Backup Time: 450ms (Adjusted)
 * - Logic: Right-Hand Rule (Prefer Right -> Then Left -> Then U-Turn)
 */

// --- HARDWARE PINS ---
const int LEFT_FORWARD   = 6;
const int LEFT_BACKWARD  = 10;
const int RIGHT_BACKWARD = 3;
const int RIGHT_FORWARD  = 5; 

// Ultrasonic Sensor
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

// --- CONFIGURATION ---
const int SPEED_L_FW   = 200; 
const int SPEED_R_FW   = 190;
const int SPEED_L_BACK = 195;
const int SPEED_R_BACK = 190; 

// STOP DISTANCE
const int WALL_DISTANCE_CM = 10;     

// --- TUNING CONSTANTS ---
const int SCAN_TURN_TIME    = 400;   // Head turn time
const int TURN_90_TIME      = 550;   // 90 deg turn time
const int TURN_180_TIME     = 1100;  // U-turn time

// *** UPDATED: BACKUP TIME ***
const int BACKUP_TIME       = 450;   

void setup() {
  Serial.begin(9600);

  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  delay(2000); 
}

void loop() {
  // 1. Scan Forward
  int distance = getDistance();
  
  // 2. Maze Logic
  // Check if distance is valid (>0) AND less than our 10cm limit
  if (distance > 0 && distance < WALL_DISTANCE_CM) {
    solveIntersection();
  } 
  else {
    moveForward();
  }
}

// ---------------------------------------------------------
// MAZE DECISION LOGIC
// ---------------------------------------------------------

void solveIntersection() {
  stopMotors();
  Serial.println("Wall at 10cm! Backing up...");
  
  // STEP 1: SAFETY BACKUP (450ms)
  moveBackward();
  delay(BACKUP_TIME);
  stopMotors();
  delay(200);

  // STEP 2: Check Right (Priority 1)
  turnRight();
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(200); 
  int rightDist = getDistance();
  
  // Return to Center
  turnLeft();
  delay(SCAN_TURN_TIME);
  stopMotors();
  
  // STEP 3: Check Left (Priority 2)
  turnLeft();
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(200);
  int leftDist = getDistance();
  
  // Return to Center
  turnRight();
  delay(SCAN_TURN_TIME);
  stopMotors();
  delay(200);

  // STEP 4: Make the Decision
  // Look for a path that has at least 20cm of open space
  
  if (rightDist > 20) {
    Serial.println("Right is Open -> Turning Right
