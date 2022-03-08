#include <Arduino.h>
#include <sk.h>

#define LED_COUNT 60
#define LED_PIN 26
#define RES_PIN 36
#define MAX_RES 4095

int Brightness = 0;
sk driver;
int State = 0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World");
  driver.begin(LED_PIN, LED_COUNT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int temp = State;
  for(int i = 0; i < LED_COUNT; i++) 
  {
    switch(temp % 4)
    {
      case 0: driver.color(i, 255, 0, 0, 0, Brightness);
      break;
      case 1: driver.color(i, 0, 255, 0, 0, Brightness);
      break;
      case 2: driver.color(i, 0, 0, 255, 0, Brightness);
      break;
      case 3: driver.color(i, 0, 0, 0, 255, Brightness);
    }
    temp++;
  }
  Brightness = (analogRead(RES_PIN) * 100) / MAX_RES;
  driver.show();
  State++;
  //Serial.println("Brightness: " + Brightness);
  delay(100);
}