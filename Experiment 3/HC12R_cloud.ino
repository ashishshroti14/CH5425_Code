#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "ThingSpeak.h"

// For wifi network connection
const char *ssid = SECRET_SSID;   // your network SSID (name) 
const char *pass = SECRET_PASS;   // your network password
WiFiClient  client;

// Secret Keys for writing to channel
unsigned long myChannelNumber = 2024462;
const char *myWriteAPIKey = "6TZ1P4S524YRO2MW";
#define DISTANCE_FIELD 1

// HC-12
#define TXPin 8
#define RXPin 7
SoftwareSerial HC12(TXPin, RXPin);

// Constants for attempting Wi-Fi reconnection
unsigned long previousMillis = 0;
unsigned long interval = 30000;


void initWifi() {
  // ESP8266 module is setup as a station and it connects to an access point
  WiFi.mode(WIFI_STA);

  // Establishing wifi connection
  WiFi.begin(ssid, pass);
  Serial.print("Attempting to connect to wifi: ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void WriteToThingSpeak(int distance) {
  // Measure Signal Strength (RSSI) of Wi-Fi connection
  long rssi = WiFi.RSSI();
  // Write value to Field 1 of a ThingSpeak Channel
  int httpCode = ThingSpeak.writeField(myChannelNumber, DISTANCE_FIELD, distance, myWriteAPIKey);

  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }
}

void setup() {
  Serial.begin(9600);             // Serial Port to computer
  while(!Serial){
    ;    
  }

  initWifi();
  ThingSpeak.begin(client);

  // Define pin modes for TX and RX
  pinMode(RXPin, INPUT);
  pinMode(TXPin, OUTPUT);  
  HC12.begin(9600);               // Software Serial port to HC12
}

void loop() {
  String data = ""
  while(HC12.available()){
    byte incomingByte = HC12.read();
    data += char(incomingByte);
  }
  Serial.println("#" + String(data));

  WriteToThingSpeak(data);

  // Wait 20 seconds to update the channel again ** 20s delay already made in Transmitter side
  //delay(20000);

}
