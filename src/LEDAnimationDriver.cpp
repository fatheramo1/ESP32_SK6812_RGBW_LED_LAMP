#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "FS.h"
#include "SPIFFS.h"
#include <sk.h>
#include <Arduino.h>
#include <stdlib.h>
#include <vector>
#include "LEDAnimation.cpp"


class LEDAnimationDriver 
{
private:
    int64_t lastCalledTime;
    int64_t frameTimeAlive;
    sk *driver;
    int pin;
    int LEDCount;
    float speedMod;
    int curAnimation;
    vector<LEDAnimation> animations;

public:
    LEDAnimationDriver(string filePath, int pin)
    {
        Serial.println("LEDDriver constructor entered");
        driver = new sk();
        this->pin = pin;
        animations.push_back(readAnimation(filePath));
        curAnimation = 0;
        frameTimeAlive = 0;
        speedMod = 1.0;
    }

    void SetStartTime(int64_t sTime) { lastCalledTime = sTime; }

    void SetSpeed(float newSpeed) { speedMod = newSpeed; }

    void SetBrightness(int brightness) { driver->setbrightness(brightness); }

    void NextAnimation()
    {
        if(curAnimation + 1 >= animations.size())
            curAnimation = 0;
        else
            curAnimation++;
    }

    bool NextAnimationStep(int64_t curtime) 
    {
        frameTimeAlive = frameTimeAlive + (curtime - lastCalledTime) * speedMod;
        lastCalledTime = curtime;
        Serial.printf("Going to LEDAnimations NextAnimationStep with TimeAlive = %d \n", frameTimeAlive);
        if(!animations.at(curAnimation).NextAnimationStep(frameTimeAlive))   //next frame started
            frameTimeAlive = 0;
        return true;
    }

    private: LEDAnimation readAnimation(string filePath)
    {
        Serial.println("ReadAnimation started");
        if(!SPIFFS.begin(true))
        {
            Serial.println("SPIFFS couldn't begin :(");
        }

        File file = SPIFFS.open("/LEDTestAnimation1.csv");
        if(!file){
            Serial.println("Failed to open file!");
        }
  
        if(!file) 
        {
            Serial.println("Oh... file is not opening/ wrong name");
            throw invalid_argument("File does not exist at " + filePath);
        }
            
        file.println();
        Serial.println("File Opened");
        String firstLine = file.readStringUntil('\n'); //first line holds general info
        vector<string> animationInfo;
        stringstream s_stream(firstLine.c_str());
        while(s_stream.good()) //breaks string at commas
        {
            string substr;
            getline(s_stream, substr, ',');
            animationInfo.push_back(substr);
        }

        int temp = std::atoi(animationInfo.at(0).c_str());
        int animationLen = StringToInt(animationInfo.at(0));
        LEDCount = StringToInt(animationInfo.at(1));
        int stepper = StringToInt(animationInfo.at(2));
        string frameEnding = animationInfo.at(3);
        vector<string> frames;
        // Serial.println("File headers read: ");
        // Serial.println(animationLen);
        // Serial.println(LEDCount);
        // Serial.println(stepper);
        // Serial.println(frameEnding.c_str());
        driver->begin(pin, LEDCount);
        for(int i = 0; i < animationLen; i++) 
        {
            //Serial.println("how many times will this print?");
            frames.push_back(file.readStringUntil('\n').c_str());
        }
        //Serial.println("Did we make it through the for loop?");
        LEDAnimation ret = LEDAnimation(LEDCount, animationLen, stepper, frameEnding, frames, driver);
        Serial.println("WE MADE IT THROUGH ALL OF THE CONSTRUCTORS");
        return ret;
    }


    //stoi for some reason doesn't work so trying this method of conversion
    public: const int StringToInt(string value) 
    {
        stringstream s(value);
        int ret;
        s >> ret;
        return ret;
    }

    public: void DebugFileOutput()
    {
      Serial.println("Starting Debug output");
      Serial.printf("PIN: %d\n", pin);
      Serial.printf("LEDCount: %d\n", LEDCount);
      Serial.printf("speedMod: %f\n", speedMod);
      Serial.printf("Num Animations: %d\n", animations.size());
      Serial.printf("curAnimation: %d\n", curAnimation);
      Serial.printf("frameTimeAlive: %d\n", frameTimeAlive);
      Serial.printf("LastCalledTime: %d\n", lastCalledTime);
      Serial.println("First Frames values");
      Serial.println("--------------------------------");
      animations.at(0).DebugFrame(0);
    }
};