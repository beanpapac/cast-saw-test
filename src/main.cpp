
#include <Arduino.h>
//#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include <time.h>


Adafruit_7segment matrix = Adafruit_7segment();
int tracker = 0;
int cutsPerSec = 5;
int pollHz = 100;


void setup() {
Serial.begin(921600);
Serial.println("7 Segment Backpack Test");
matrix.begin(0x70);
pinMode(A7, INPUT);
}

int codeDelay = int(1000/pollHz);
int readDelay = int(1000/cutsPerSec);

void loop() {
  //int sensorValue = analogRead(A9);
  //int pinval = analogRead(A7);
  //Serial.println(pinval);

  // if the circuit is completed and A7 is pulled high record a cut and sleep until the next cut can be recorded
  if(digitalRead(A7))
  {
    tracker++;
    delay(readDelay);
    matrix.print(tracker, DEC);
    matrix.writeDisplay();
  }
  else
  {
    delay(codeDelay);
    matrix.print(tracker, DEC);
    matrix.writeDisplay();
  }

  // reset tracker value before it gets too large for display
  if(tracker > 9997)
  {
    tracker = 0;
  }

  // Puts the Esp32 into deep sleep mode after 20 min, increases battery life by 40x
   if(millis() > 1200000)
  {
    esp_deep_sleep_start();
  }
}