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
#include "LEDAnimation.h"

using namespace std;

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
    LEDAnimationDriver(string filePath, int pin);
    void SetStartTime(int64_t sTime) { lastCalledTime = sTime; }
    void SetSpeed(float newSpeed) { speedMod = newSpeed; }
    void SetBrightness(int brightness) { driver->setbrightness(brightness); }
    void NextAnimation();
    bool NextAnimationStep(int64_t curtime);
    void DebugFileOutput();

private: 
    LEDAnimation readAnimation(string filePath);

//stoi for some reason doesn't work so trying this method of conversion
public: const int StringToInt(string value)
    {
        stringstream s(value);
        int ret;
        s >> ret;
        return ret;
    }

};