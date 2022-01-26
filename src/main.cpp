
#include <Arduino.h>
//#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include <time.h>


Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
Serial.begin(921600);
Serial.println("7 Segment Backpack Test");
matrix.begin(0x70);
pinMode(A7, INPUT);

}

int tracker = 0;
void loop() {
  int sensorValue = analogRead(A9);
  //int pinval = analogRead(A7);

  //Serial.println(pinval);

  if(digitalRead(A7))
  {
    tracker++;
    delay(200);
  }
  if(tracker > 9000)
  {
    tracker = 0;
  }

  // if(digitalRead(A7))
  // {
  //   tracker = 0;
  //   Serial.println("true");
  // }
  // else
  // {
  //   //Serial.println("false");
  // }

  matrix.print(tracker, DEC);
  matrix.writeDisplay();
  delay(10);

  // Puts the Esp32 into deep sleep mode after 20 min, increases battery life by 40x
  if(millis() > 1200000)
  {
    esp_deep_sleep_start();
  }
  // boolean drawDots = true;
  // // try to print a number thats too long
  // matrix.print(10000, DEC);
  // matrix.writeDisplay();
  // delay(500);

  // // print a hex number
  // matrix.print(0xBEEF, HEX);
  // matrix.writeDisplay();
  // delay(500);

  // // print a floating point 
  // matrix.print(12.34);
  // matrix.writeDisplay();
  // delay(500);

  // // print a string message
  // matrix.println("BEef");
  // matrix.drawColon(drawDots);
  // matrix.writeDisplay();
  // delay(100000);
 
  

}