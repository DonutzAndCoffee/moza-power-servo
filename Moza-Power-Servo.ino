#include <Servo.h>

// Create servo instance
Servo myServo;

// Pin configuration
const int servoPin = 9;        // Servo connected to digital pin D9
const long BAUD = 115200;      // Serial baud rate

// Define your desired "HOME" position here:
const int HOME_ANGLE = 30;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.setTimeout(50);  // Timeout for readString()

  // Set servo pin to a defined LOW output state first
  // This prevents floating signals during boot
  pinMode(servoPin, OUTPUT);
  digitalWrite(servoPin, LOW);

  // Short boot delay to allow USB/MCU stabilization
  delay(200);

  // Attach servo and immediately move to HOME position
  myServo.attach(servoPin);
  delay(100);                  // Small delay to ensure stable attach
  myServo.write(HOME_ANGLE);   // Move to HOME position
  
  // Optional: write HOME a second time
  // Some small servos (e.g. SG90) behave more stable this way
  delay(100);
  myServo.write(HOME_ANGLE);

  // Detach servo to remove holding torque and avoid jitter
  myServo.detach();

  Serial.println(F("READY. Send angle 0..180 (e.g., 0, 90, 180). Type SWEEP to test."));
}

void loop() {
  if (Serial.available() > 0) {

    // Read full input (numbers or text), works even without newline
    String in = Serial.readString();
    in.trim();          // Remove leading/trailing whitespace
    in.toUpperCase();   // Convert to uppercase for case-insensitive commands

    // Test sweep command
    if (in == "SWEEP") {
      Serial.println(F("SWEEP start"));

      // Sweep from 0° to 180°
      for (int a = 0; a <= 180; a += 5) {
        myServo.write(a);
        delay(20);
      }

      // Sweep back from 180° to 0°
      for (int a = 180; a >= 0; a -= 5) {
        myServo.write(a);
        delay(20);
      }

      // Return to center position (90°)
      myServo.attach(servoPin);
      delay(100);
      myServo.write(90);
      
      Serial.println(F("SWEEP done"));

      delay(100);
      myServo.detach();
      return;
    }

    // Try to parse an angle from the input
    // toInt() returns 0 if nothing valid was found
    long angle = in.toInt();

    // Special case: detect if user explicitly entered "0"
    bool inputIsZero = (in == "0");

    // Accept valid numeric input
    if (angle != 0 || inputIsZero) {

      // Constrain angle to valid servo range
      angle = constrain(angle, 0, 180);

      // Attach servo, move to requested position, then detach again
      myServo.attach(servoPin);
      delay(100);
      myServo.write((int)angle);

      Serial.print(F("OK angle="));
      Serial.println((int)angle);

      delay(100);
      myServo.detach();
    } 
    else {
      // Invalid input handling
      Serial.print(F("IGNORED input: '"));
      Serial.print(in);
      Serial.println(F("' (not a number, try 0..180 or SWEEP)"));
    }
  }
}
