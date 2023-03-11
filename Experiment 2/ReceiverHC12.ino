#include <SoftwareSerial.h>

// HC-12
#define TXPin 4
#define RXPin 0
SoftwareSerial HC12(TXPin, RXPin);

void setup() {
  Serial.begin(9600);             // Serial Port to computer
  
// Define pin modes for TX and RX
  //pinMode(RXPin, INPUT);
  //pinMode(TXPin, OUTPUT);  
  HC12.begin(9600);               // Software Serial port to HC12
}

void loop() {
  
   Serial.println((HC12.readString()));

}
