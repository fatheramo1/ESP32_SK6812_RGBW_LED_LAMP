#include <Arduino.h>
#include "LEDAnimationDriver.h"
#include <sk.h>
#include <sys/time.h>
#include "SPIFFS.h"
#include <iostream>
#include <string>
//#include <filesystem>
#include <unistd.h>

#define LED_COUNT 60
#define LED_PIN 26
#define RES_PIN_1 36
#define RES_PIN_2 39
#define MAX_RES 4095

int Brightness = 0;
LEDAnimationDriver *LEDDriver;
char tmp[256];

/**
 * @brief Get the Time In Milli-Second precision 
 * 
 * @return int64_t current time
 */
int64_t GetTimeInMilSec() {
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    int64_t ret = (int64_t)tv_now.tv_sec * 1000000L + (int64_t)tv_now.tv_usec;//microsecond percesion
    ret = ret / 1000;//millisecond percession
    return ret;
}

/**
 * @brief Takes in a ADCs analog voltage and converts it to a value between 0.5 and 10.
 * Used to convert the ADCs value to a smaller float that can be used as a speed scaller.
 * 
 * @param adcVal 
 * @return float 
 */
float ADCToFloat(int adcVal)
{
  float ret = float(adcVal) * 10 / MAX_RES + 0.5;
  return ret;
}

void setup() 
{
  //Testing Block for file system
  
  Serial.begin(9600);
  Serial.println("Hello World");
  /*
  if(!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS couldn't begin :(");
    return;
  }

  File file = SPIFFS.open("/PurpleDotAnimation.csv");
  if(!file){
    Serial.println("Failed to open file!");
    return;
  }
  
  Serial.println("Content of file:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
  */


  //Anitialize the LEDAnimation Driver with a desired LEDAnimation csv file.
  try
  {
    LEDDriver = new LEDAnimationDriver("/PurpleDotAnimation.csv", LED_PIN);
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

/**
 * @brief The Loop simply gets the current time.
 * Reads the Analog pins for setting the drivers brightness and speed.
 * Then called the driver to compute and show the next step
 * in the animation at the current time.
 * 
 */
void loop() 
{
  try
  {
    int64_t time = GetTimeInMilSec();
    //Serial.printf("NextStep time is %d\n", time);
    LEDDriver->SetBrightness((analogRead(RES_PIN_1) * 100) / MAX_RES);
    LEDDriver->SetSpeed((ADCToFloat(analogRead(RES_PIN_2))));
    LEDDriver->NextAnimationStep(time);
  }
  catch (exception e)
  {
    Serial.println(e.what());
    delay(100000);
  }
  //delay(10);
}