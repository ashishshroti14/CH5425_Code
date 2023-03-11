#include <SoftwareSerial.h>

// HC-SR04 Ultrasonic sensor
// attach pin D3 Arduino to pin Echo of HC-SR04
#define echoPin 3
//attach pin D2 Arduino to pin Trig of HC-SR04
#define trigPin 2
// variable for the distance measurement
int distance;

// HC-12
#define TXPin 8
#define RXPin 7
SoftwareSerial HC12(TXPin, RXPin);

// Setting up HC-SR04 Utrasonic distance sensor
void setupHCSR04(){
  // Sets the trigPin as an Output
  pinMode(trigPin, OUTPUT);
  // Sets the echoPin as an Input
  pinMode(echoPin, INPUT);
}

// Reading from HC-SR04 Ultrasonic sensor
float readHCSR04(){
  int duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echo pin and records duration in microseconds when it is turned to value of second argument(here, HIGH)
  duration = pulseIn(echoPin, HIGH);
  // We take half because sound waves travel to the object and back
  distance = duration / 58;
  return distance;
}

void setup() {
  Serial.begin(9600);             // Serial Port to computer

  HC12.begin(9600);               // Serial port to HC12

  setupHCSR04();
}

void loop() {
  distance = readHCSR04();
  Serial.print("#"+String(distance));
  char payload[10], data[15];
  itoa(distance, payload, 10);  
  sprintf(data, "#%s", payload);
  
  HC12.write(data);
  delay(5000);  
}
