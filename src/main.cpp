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
#define BRIGHTNESS_PIN 36
#define SPEED_PIN 39
#define BUTTON_PIN 33 //TODO:Pick pin
#define MAX_RES 4095
#define COUNT_LONG_PRESS 50

int Brightness = 0;
bool On = false;
LEDAnimationDriver *LEDDriver;
char tmp[256];

TaskHandle_t *checkInputs = NULL;

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

void CheckInputs(void *parameter);

void setup() 
{
  //Testing Block for file system
  
  Serial.begin(9600);
  Serial.println("Hello World");
  pinMode(BUTTON_PIN, INPUT);


  //Anitialize the LEDAnimation Driver with a desired LEDAnimation csv file.
  try
  {
    LEDDriver = new LEDAnimationDriver("/PurpleDotAnimation.csv", LED_PIN);
    //Add all animations below
    //-----------------------------------------------------------
    LEDDriver->AddAnimation("/LEDTestAnimation1.csv");
    LEDDriver->AddAnimation("/RedShiftWhite.csv");
    //-----------------------------------------------------------
    LEDDriver->DebugFileOutput();
    LEDDriver->ClearAnimation();
    LEDDriver->SetStartTime(GetTimeInMilSec());
  }
  catch (exception e)
  {
    Serial.println(e.what());
  }

  xTaskCreate(
    CheckInputs,
    "CheckInputs",
    1000,
    NULL,
    1,
    checkInputs
  );
}

void CheckInputs(void *parameter)
{
  int8_t countPressed = 0;
  for(;;)
  {

    try
    {
      if(digitalRead(BUTTON_PIN) == HIGH)
      {
        countPressed++;
        if(countPressed == COUNT_LONG_PRESS)
        {
          //Long press
          Serial.println("Long Press");
          LEDDriver->ClearAnimation();
          LEDDriver->SetStartTime(GetTimeInMilSec());
          On = !On;
        }
      }
      else
      {
        if(countPressed > 0 && countPressed < COUNT_LONG_PRESS)
        {
          //Short press
          Serial.println("Short Press");
          if(On)
            LEDDriver->NextAnimation(GetTimeInMilSec());
          else
            On = true;
        }
        //reset count
        countPressed = 0;
      }

      if(On)
      {
        LEDDriver->SetBrightness((analogRead(BRIGHTNESS_PIN) * 100) / MAX_RES);
        LEDDriver->SetSpeed((ADCToFloat(analogRead(SPEED_PIN))));
        LEDDriver->NextAnimationStep(GetTimeInMilSec());
      }
    }
    catch (exception e)
    {
      countPressed = 0;
      Serial.println(e.what());
      delay(100000);
    }
    delay(10);
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
  //LEDDriver->NextAnimationStep(GetTimeInMilSec());
  delay(10000);
}