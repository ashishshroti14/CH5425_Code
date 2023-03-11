#include <SD.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

// SD card
File sdcard_file;
// Pin where we have connected Chip select pin of SD card.
const int chipSelect = 10;

// HC-SR04 Ultrasonic sensor
// attach pin D3 Arduino to pin Echo of HC-SR04
#define echoPin 3
//attach pin D2 Arduino to pin Trig of HC-SR04
#define trigPin 2
// variable for the duration of sound wave travel and for the distance measurement
float duration, distance;

// TEMT6000 Analog sensor
// Ambient light sensor pin
#define LIGHTSENSORPIN A0

// Adafruit bmp280 sensor
Adafruit_BMP280 bmp; // I2C
float temperature, pressure;

// Initialising the SD card.
bool setupSDCard(){
  pinMode(chipSelect, OUTPUT);
  if(!SD.begin()){
    Serial.println("SD card initialization failed");
    return false;
  }
  return true;
}

// Setting up HC-SR04 Utrasonic distance sensor
void setupHCSR04(){
  // Sets the trigPin as an Output
  pinMode(trigPin, OUTPUT);
  // Sets the echoPin as an Input
  pinMode(echoPin, INPUT);
}

// Setting up the BPM280 pressure and temperature sensor.
bool setupBMP280(){
  unsigned status = bmp.begin(118);
  if (!status) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring or "
                   "try a different address!");
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    return false;
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  return true;
}

void setup() {
  // Open communications between 2 devices using a serial protocol.

  pinMode(LIGHTSENSORPIN,  INPUT);  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if(!setupSDCard()){
    while(1);
  }
  Serial.println("SD card is ready to use.");

  setupHCSR04();
  Serial.println("HC-SR04 sensor is configured successfully.");

  if(!setupBMP280()){
    while(1);    
  }
  Serial.println("BMP280 sensor is configured successfully.");

  // Setting up TEMT6000 Analog sensor
  pinMode(LIGHTSENSORPIN, INPUT);
}

// Reading from HC-SR04 Ultrasonic sensor
float readHCSR04(){
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

void loop() {
  distance = readHCSR04();

  // Reading from BMP280 sensor
  // Temperature is returned in C
  temperature = bmp.readTemperature();
  // Pressuer which is returned in Pa is converted into atm
  pressure = bmp.readPressure() * 0.00000987;

  // Reading from TEMT6000 Analog sensor
  float light = analogRead(LIGHTSENSORPIN); //Read light level
  // Serial.println(String(light )+ "light");
  // Open the file to be written in SD card.
  sdcard_file = SD.open("Datash3.txt", FILE_WRITE);
  if (sdcard_file) {
    // Serial.println(String(distance));
    // Serial.println(String(light));    
    // Serial.println("$"+String(distance)+"#"+String(light)+"@"+String(temperature)+"&"+String(pressure)+"\n");
    Serial.print("$");
    Serial.print(String(distance));
    Serial.print("#");
    Serial.print(String(light));
    Serial.print("@");
    Serial.print(String(temperature));
    Serial.print("&");
    Serial.print(String(pressure));
    Serial.print("\n");

    sdcard_file.print("$");
    sdcard_file.print(String(distance));
    sdcard_file.print("#");
    sdcard_file.print(String(light));
    sdcard_file.print("@");
    sdcard_file.print(String(temperature));
    sdcard_file.print("&");
    sdcard_file.print(String(pressure));
    sdcard_file.print("\n");


    

    // sdcard_file.print("$"+String(distance)+"#"+String(light)+"@"+String(temperature)+"&"+String(pressure)+"\n");
    sdcard_file.close();
  }else{
    Serial.println("Error opening datalog.txt");
  }

  // We need to log every 5 seconds
  delay(5000);
}
