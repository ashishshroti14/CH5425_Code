#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <SoftwareSerial.h>



Adafruit_MPU6050 mpu;

const int MPU_addr=0x68;

float gyroX=0;
float gyroY=0;
float gyroZ=0;
float xAnglVel, yAnglVel, zAnglVel;

SoftwareSerial HC12(8, 7);

void setup() {
  Serial.begin(115200);
  HC12.begin(9600);               // Serial port to HC12

  //  Try to initialize!
   if (!mpu.begin()) {
     Serial.println("Failed to find MPU6050 chip");
     while (1);
   }

   Serial.println("MPU6050 Found!");

  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
	mpu.setGyroRange(MPU6050_RANGE_500_DEG);
	mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

int count=0;
volatile long int myTimeOld=millis();
volatile long int myTimeOld2=millis();
sensors_event_t a, g, t;

int min_width=3;
int num_digits_after_decimal=3;

char strXAnglVel[20], strYAnglVel[20], strZAnglVel[20], data[60];

void loop() {
	
	int myTimeNew = millis();

	
	
	if(myTimeNew-myTimeOld >= 50){
		mpu.getEvent(&a, &g, &t);
		gyroX=gyroX + g.gyro.x;
		gyroY=gyroY + g.gyro.y;
		gyroZ=gyroZ + g.gyro.z;

		count +=1;
		
		myTimeOld=myTimeNew;
	}

	if(myTimeNew-myTimeOld2 >= 2000){

		xAnglVel=gyroX/count;
		yAnglVel=gyroY/count;
		zAnglVel=gyroZ/count;

		gyroX=0;
		gyroY=0;
		gyroZ=0;
		count=0;

		dtostrf(xAnglVel, min_width, num_digits_after_decimal, strXAnglVel );
		dtostrf(yAnglVel, min_width, num_digits_after_decimal, strYAnglVel );
		dtostrf(zAnglVel, min_width, num_digits_after_decimal, strZAnglVel );

		sprintf(data, "#%s@%s$%s", strXAnglVel, strYAnglVel, strZAnglVel);

		HC12.write(data);
	}	
}