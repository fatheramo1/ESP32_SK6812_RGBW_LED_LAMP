#include <Arduino.h>
#include <LEDAnimationDriver.cpp>
#include <sk.h>
#include <sys/time.h>
#include "SPIFFS.h"
#include <iostream>
#include <string>
//#include <filesystem>
#include <unistd.h>

#define LED_COUNT 60
#define LED_PIN 26
#define RES_PIN 36
#define MAX_RES 4095

int Brightness = 0;
LEDAnimationDriver *LEDDriver;
char tmp[256];

int64_t GetTimeInMilSec() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    int64_t ret = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;//microsecond percesion
    ret = ret / 1000;//millisecond percession
    return ret;
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS couldn't begin :(");
    return;
  }

  File file = SPIFFS.open("/LEDTestAnimation1.csv");
  if(!file){
    Serial.println("Failed to open file!");
    return;
  }
  
  Serial.println("Content of file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
  try
  {
    LEDDriver = new LEDAnimationDriver("LEDTestAnimation1.csv", LED_PIN);
    int64_t time = GetTimeInMilSec();
    Serial.printf("Setting start time as %d\n", time);
    LEDDriver->SetStartTime(time);
    LEDDriver->DebugFileOutput();
  }
  catch (exception e)
  {
    Serial.println(e.what());
  }

}

void loop() 
{
  try
  {
    int64_t time = GetTimeInMilSec();
    Serial.printf("NextStep time is %d\n", time);
    LEDDriver->NextAnimationStep(time);
    //Serial.println("We got through NextAnimationStep");
    LEDDriver->SetBrightness((analogRead(RES_PIN) * 100) / MAX_RES);
    //Serial.println("We got through SetBrightness");
  }
  catch (exception e)
  {
    Serial.println(e.what());
    delay(100000);
  }
  delay(10);
}

// int64_t GetTimeInMilSec() {
//     struct timeval tv_now;
//     gettimeofday(&tv_now, NULL);
//     int64_t ret = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;//microsecond percesion
//     ret = ret / 1000;//millisecond percession
//     return ret;
// }