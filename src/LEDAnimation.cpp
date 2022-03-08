#include <sk.h>
#include <Arduino.h>
#include <vector>
#include <LEDFrame.h>

using namespace std;

class LEDAnimation 
{
    private:
    vector<LEDFrame> animation;
    int curFrame; //pointer to current frame to display within the animation
    int nextFrame;
    int stepper; //used to figure out how to increment nextFrame
    int animationLen;
    int LEDCount;
    int pin;
    float speedMod;
    time_t lastCalledTime;
    time_t timeAlive;
    sk driver;

    public:
    //Allocating memory for animation based on desired LEDCount and AnimationLength
    LEDAnimation(int LEDCount, int animationLen, int pin) 
    {
        this->pin = pin;
        this->animationLen = animationLen;
        this->LEDCount = LEDCount;
        timeAlive = 0;
        curFrame = 0;
        nextFrame = 1; //should do some checking before assuming 1 exists
        stepper = 1;
        speedMod = 1.0;
        driver.begin(pin, LEDCount);
    }

    void SetStartTime(time_t sTime) { lastCalledTime = sTime; }

    void SetSpeed(float newSpeed) { speedMod = newSpeed; }

    bool NextAnimationStep(time_t time) 
    {
        time_t timeAlive = timeAlive + (time - lastCalledTime) * speedMod;
        if(timeAlive >= animation.at(curFrame).GetTTL())
        {
            timeAlive = 0;
            curFrame = nextFrame;
            if((nextFrame + stepper) >= 0 && (nextFrame + stepper) < animationLen) {
                nextFrame = nextFrame + stepper;
            }
            else
            {
                //set up switch with each case being one of the enums for how to decide what to do at the end of the animation
            }
        }
    }

    void ShowFrame() {driver.show();}

/*
    void NextFrame()
    {
        if()
    }
    */
};