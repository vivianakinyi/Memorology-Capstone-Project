/*
Reference: 
Wise Tech (2015) https://duino4projects.com/smart-snow-globe-gives-snow-day-alerts-using-arduino/
Arduino Built-in Example: Fade (last revisioned in 2018)https://www.arduino.cc/en/Tutorial/BuiltInExamples/Fade
ADXL335 Accelerometer Library by Frankie Chu for seeed studio (last revised in 2021) https://github.com/Seeed-Studio/Accelerometer_ADXL335

*/
#include <Wire.h>
#include <math.h>
#include <Arduino.h>
#include "ADXL335.h"

int led = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 51;    // how many points to fade the LED by

ADXL335 accelerometer;

// Sensitivity threshold for the accelerometer.  Higher numbers
// make it less sensitive (i.e. you have to shake it more for
// it to register as shaken.  A value of 1 is about right.
float sensitivity = 1.3;

void setup()
{
  Serial.begin(9600);
  accelerometer.begin();  // initialize the accelerometer
  pinMode(led, OUTPUT);   // declare pin 9 to be an output
}
 
void loop()
{
  if (shake()) {
   // set the brightness of pin 9:
   if (brightness < 255){
      brightness = brightness + fadeAmount;
      analogWrite(led, brightness);
      delay(50);
    }
  } else {
    if (brightness >= 51){
      brightness = brightness - fadeAmount;
      analogWrite(led, brightness);
      delay(50);
      }
    }
}

boolean shake() {
   float ax, ay, az;
   accelerometer.getAcceleration(&ax, &ay, &az);
   Serial.println("accleration of X/Y/Z: ");
   Serial.print(ax);
   Serial.println(" g");
   Serial.print(ay);
   Serial.println(" g");
   Serial.print(az);
   Serial.println(" g");
   delay(500);
  

    if((abs(ax) > sensitivity) && (abs(ay) > sensitivity) && (abs(az) > sensitivity)) {
    Serial.println("shaken!");
    return true;
  } else {
    Serial.println("still");
    return false;
  }
}
