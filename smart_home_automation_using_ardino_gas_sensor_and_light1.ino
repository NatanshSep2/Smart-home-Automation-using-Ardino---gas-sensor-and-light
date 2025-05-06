// Include Servo motor library
#include <Servo.h> 

// Declare variables
int output1Value = 0;
int sen1Value = 0;
int sen2Value = 0;
const int gas_sensor = A1;  // Gas sensor connected to A1
const int LDR = A0;         // LDR sensor connected to A0
int limit = 400;            // Threshold limit for gas detection

// Function to read distance from Ultrasonic Sensor
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);         // Set trigger pin as output
  digitalWrite(triggerPin, LOW);       // Clear the trigger
  delayMicroseconds(2);
  
  digitalWrite(triggerPin, HIGH);      // Send a 10 microseconds pulse
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);              // Set echo pin as input
  return pulseIn(echoPin, HIGH);         // Read and return pulse duration
}

// Create Servo object
Servo servo_7; 

// Setup function (runs once)
void setup()
{
  Serial.begin(9600);             // Start serial communication at 9600 baud rate
  
  pinMode(A0, INPUT);              // LDR sensor input
  pinMode(A1, INPUT);              // Gas sensor input
  pinMode(13, OUTPUT);             // Relay output (for light control)
  
  servo_7.attach(7, 500, 2500);     // Attach servo motor to pin 7

  pinMode(8, OUTPUT);               // Buzzer output
  pinMode(9, INPUT);                // PIR motion sensor input
  pinMode(10, OUTPUT);              // NPN transistor (Fan) control
  pinMode(4, OUTPUT);               // Red LED output (no motion)
  pinMode(3, OUTPUT);               // Green LED output (motion detected)
}

// Main loop (runs continuously)
void loop()
{
  // ---- Light Control based on LDR ----
  int val1 = analogRead(LDR);        // Read LDR value
  
  if (val1 > 500) 
  {
    digitalWrite(13, LOW);           // Turn ON light if it's dark
    Serial.print("Bulb ON = ");
    Serial.print(val1);
  } 
  else 
  {
    digitalWrite(13, HIGH);          // Turn OFF light if it's bright
    Serial.print("Bulb OFF = ");
    Serial.print(val1);
  }

  // ---- Motion Detection using PIR Sensor ----
  sen2Value = digitalRead(9);         // Read PIR sensor value
  
  if (sen2Value == 0) 
  {
    digitalWrite(10, LOW);            // Turn OFF Fan (NPN OFF)
    digitalWrite(4, HIGH);            // Turn ON Red LED (no motion)
    digitalWrite(3, LOW);             // Turn OFF Green LED
    Serial.print(" || NO Motion Detected ");
  }
  else if (sen2Value == 1) 
  {
    digitalWrite(10, HIGH);           // Turn ON Fan (NPN ON)
    delay(5000);                      // Keep fan ON for 5 seconds
    digitalWrite(4, LOW);             // Turn OFF Red LED
    digitalWrite(3, HIGH);             // Turn ON Green LED
    Serial.print(" || Motion Detected! ");
  }

  // ---- Gas Leakage Detection using MQ2 Sensor ----
  int val = analogRead(gas_sensor);   // Read gas sensor value
  Serial.print(" || Gas Sensor Value = ");
  Serial.print(val);

  if (val > limit) 
  {
    tone(8, 650);                     // Sound buzzer at 650 Hz
  }
  delay(300);                         // Wait 300 ms
  noTone(8);                          // Turn OFF buzzer

  // ---- Distance Measurement and Door Automation using Ultrasonic Sensor ----
  sen1Value = 0.01723 * readUltrasonicDistance(6, 6); // Convert pulse time to distance (in cm)

  if (sen1Value < 100) 
  {
    servo_7.write(90);                // Open door (rotate servo 90 degrees)
    Serial.print(" || Door Open! ; Distance = ");
    Serial.print(sen1Value);
    Serial.print("\n");
  } 
  else 
  {
    servo_7.write(0);                 // Close door (rotate servo to 0 degrees)
    Serial.print(" || Door Closed! ; Distance = ");
    Serial.print(sen1Value);
    Serial.print("\n");
  }

  delay(10); // Short delay to improve simulation performance
}
