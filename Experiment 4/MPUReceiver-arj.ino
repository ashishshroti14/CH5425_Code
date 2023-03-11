#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include "ThingSpeak.h"

// For wifi network connection
const char *ssid = "Karoly17";   // your network SSID (name) 
const char *pass = "abcd1234";   // your network password
WiFiClient  client;

// Secret Keys for writing to channel
unsigned long myChannelNumber = 3; //2031559
const char *myWriteAPIKey = "82R3AZUFJFVHZEHR";
#define DISTANCE_FIELD 1

// HC-12
#define TXPin 4
#define RXPin 0
SoftwareSerial HC12(TXPin, RXPin);

// Constants for attempting Wi-Fi reconnection
unsigned long previousMillis = 0;
unsigned long interval = 30000;


void initWiFi() {
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

void WriteToThingSpeak(float xAngVel, float yAngVel, float zAngVel) {
  

  ThingSpeak.setField(1, xAngVel);
  ThingSpeak.setField(2, yAngVel);
  ThingSpeak.setField(3,zAngVel);

  int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

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

  initWiFi();
  ThingSpeak.begin(client);

  // Define pin modes for TX and RX
  pinMode(RXPin, INPUT);
  pinMode(TXPin, OUTPUT);  
  HC12.begin(9600);               // Software Serial port to HC12
}


myOldTime=millis();

void loop() {
  String incomingByte = HC12.readString();
  Serial.println(String(HC12.readString()));
  
  int i=0;
  while(incomingByte[i] != "#"){
    i +=1;
  }

  int j=0;

  while(incomingByte[j] != "@"){
    j +=1;
  }

  int k=0;
  while(incomingByte[k] != "$"){

    k +=1;
  }

    xAngVel = incomingByte.substring(i+1,j-1);
    yAngVel = incomingByte.substring(j+1,k-1);
    zAngVel = incomingByte.substring(k+1);

  String data=incomingByte.substring(1,3);
  Serial.println(data);

    myNewTime = millis();

    if(myNewTime-myOldTime >=20000){

      WriteToThingSpeak(xAngVel, yAngVel, zAngVel);

      myOldTime=myNewTime;
    }

}
