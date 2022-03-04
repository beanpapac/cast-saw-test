
#include <Arduino.h>
//#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <SPI.h>
#include <time.h>
//#include <SD.h>
#include <RTClib.h>

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Adafruit_7segment matrix = Adafruit_7segment();
int tracker = 0;
int cutsPerSec = 5;
int pollHz = 100;
#define FILE_BASE_NAME "Data"


void setup() {
  Serial.begin(921600);
  Serial.println("7 Segment Backpack Test");
  matrix.begin(0x70);
  pinMode(A7, INPUT);
  setCpuFrequencyMhz(20);

  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
    

  rtc.start();

  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours
  // float drift_unit = 4.069; //For corrections every min the drift_unit is 4.069 ppm (use with offset mode PCF8523_OneMinute)
  int offset = round(deviation_ppm / drift_unit);

    
  Serial.print("Offset is "); Serial.println(offset); // Print to control offset
}

int codeDelay = int(1000/pollHz);
int readDelay = int(1000/cutsPerSec);

void loop() {
  //int sensorValue = analogRead(A9);
  //int pinval = analogRead(A7);
  //Serial.println(pinval);


  DateTime now = rtc.now();

  // Serial.print(now.year(), DEC);
  // Serial.print('/');
  // Serial.print(now.month(), DEC);
  // Serial.print('/');
  // Serial.print(now.day(), DEC);
  // Serial.print(" (");
  // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  // Serial.print(") ");
  // Serial.print(now.hour(), DEC);
  // Serial.print(':');
  // Serial.print(now.minute(), DEC);
  // Serial.print(':');
  // Serial.print(now.second(), DEC);
  // Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

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