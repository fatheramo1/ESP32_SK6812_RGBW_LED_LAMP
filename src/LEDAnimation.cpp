#include <sk.h>
#include <Arduino.h>
#include <stdlib.h>
#include <vector>
#include <LEDFrame.h>

using namespace std;

/**
 * @brief Used to decide what to do when an animation reaches the end of a frame
 * 
 */
enum EndingType {
    REVERSE,
    RESTART,
    RANDOM
};

class LEDAnimation 
{
    private:
    vector<LEDFrame> animation;
    int curFrame; //pointer to current frame to display within the animation
    int nextFrame;
    int stepper; //used to figure out how to increment nextFrame
    int animationLen;
    sk* driver;
    EndingType frameEnding;

    //TODO: figure out how to convert the string from the file to an enum
    private: EndingType GetEndingType(string ending)
    {
        EndingType ret = RESTART;
        // switch(ending)
        // {
        //     case "reverse": ret = REVERSE;
        //     break;
        //     case "restart": ret = RESTART;
        //     break;
        //     case "random" : ret = RANDOM;
        //     break;
        // }
        return ret;
    }
    

    public:
    //Allocating memory for animation based on desired LEDCount and AnimationLength
    LEDAnimation(int LEDCount, int animationLen, int stepper, string ending, vector<string> frames, sk *driver) 
    {
        this->animationLen = animationLen;
        this->stepper = stepper;
        frameEnding = GetEndingType(ending);
        curFrame = 0;
        nextFrame = 1; //should do some checking before assuming 1 exists
        this->driver = driver;
        //Serial.println("Got through basic assignments");

        for(int i = 0; i < animationLen; i++) 
        {
            //Serial.println("For loop within LEDAnimation constructor");
            animation.push_back(LEDFrame(frames.at(i), driver));
        }
        //Serial.println("LEDAnimation constructor is finished");
    }


    bool NextAnimationStep(int64_t timeAlive) 
    {
        Serial.printf("Made it to LEDAnimation's NextAnimationStep with %d TA\n", timeAlive);
        Serial.printf("curFrame = %d\n", curFrame);
        Serial.println("curFrame Values");
        animation.at(curFrame).DebugFrame();
        Serial.printf("nextFrame = %d\n", nextFrame);
        Serial.println("nextFrame Values");
        animation.at(nextFrame).DebugFrame();
        bool ret = true;
        if(timeAlive >= animation.at(curFrame).GetTTL())
        {
            ret = false;
            PickNextFrame();
        }
        animation.at(curFrame).ShowColorsAt(timeAlive, animation.at(nextFrame).GetLEDValues());
        return ret;
    }

    private:
    bool PickNextFrame() 
    {
        bool ret = false;
        switch (frameEnding) 
        {
            case RESTART : ret = restartEnding();
            break;
            case REVERSE : ret = reverseEnding();
            break;
            case RANDOM : ret = randomEnding();
            break;
        }
        return ret;
    }

    bool restartEnding() 
    {
        curFrame = nextFrame;
        if((nextFrame + stepper) < animationLen) 
        {
            nextFrame = nextFrame + stepper;
        }
        else
        {
            nextFrame = 0;
        }
        return true;
    }

    bool reverseEnding() 
    {
        curFrame = nextFrame;
        if((nextFrame + stepper) >= 0 && (nextFrame + stepper) < animationLen) 
        {
            nextFrame = nextFrame + stepper;
        }
        else
        {
            stepper = stepper * -1;
            nextFrame = nextFrame + stepper;
        }
        return true;
    }

    bool randomEnding()
    {
        curFrame = nextFrame;
        nextFrame = rand() % animation.size();
    }

    void ShowFrame() {driver->show();}

    public: void DebugFrame(int frameNum)
    {
        animation.at(frameNum).DebugFrame();
    }

};