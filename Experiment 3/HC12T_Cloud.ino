#include <SoftwareSerial.h>

// HC-SR04 Ultrasonic sensor
// attach pin D3 Arduino to pin Echo of HC-SR04
#define echoPin 3
//attach pin D2 Arduino to pin Trig of HC-SR04
#define trigPin 2
// variable for the duration of sound wave travel and for the distance measurement
float duration, distance;

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
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echo pin and records duration in microseconds when it is turned to value of second argument(here, HIGH)
  duration = pulseIn(echoPin, HIGH);
  // We take half because sound waves travel to the object and back
  distance = (duration * 0.034) / 2;
  return distance;
}

void setup() {
  Serial.begin(9600);             // Serial Port to computer
  while(!Serial){
    ;    
  }


  // Define pin modes for TX and RX
  pinMode(RXPin, INPUT);
  pinMode(TXPin, OUTPUT);
  HC12.begin(9600);               // Serial port to HC12

  setupHCSR04();
}

void loop() {
  distance = readHCSR04();
  Serial.print("#"+String(distance));
  
//  while(Serial.available()){
//    HC12.write(Serial.read());   
//  }
  
  //Serial.available() checkss for number of bytes available from reading. I don't think we need this function in transmitter.

  HC12.write(distance); 
  delay(20000);  
}
