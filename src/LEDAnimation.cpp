#include "LEDAnimation.h"

//TODO: figure out how to convert the string from the file to an enum
LEDAnimation::EndingType LEDAnimation::GetEndingType(string ending)
{
    Serial.printf("Ending name found: |%s|\n", ending.c_str());
    EndingType ret = RESTART;
    if(ending == "restart")
    {
        ret = RESTART;
        Serial.println("restart found");
    }
    else if(ending == "reverse")
    {
        ret = REVERSE;
        Serial.println("reverse found");
    }
    else if(ending == "random")
    {
        ret = RANDOM;
        Serial.println("random found");
    }
    return ret;
}


//Allocating memory for animation
LEDAnimation::LEDAnimation(int stepper, string ending, vector<string> frames, sk *driver) 
{
    this->stepper = stepper;
    frameEnding = GetEndingType(ending);
    curFrame = 0;
    nextFrame = 1; //should do some checking before assuming 1 exists
    //Serial.println("Got through basic assignments");
    for(int i = 0; i < frames.size(); i++) 
    {
        //Serial.println("For loop within LEDAnimation constructor");
        animation.push_back(LEDFrame(frames.at(i), driver));
    }
    //Serial.println("LEDAnimation constructor is finished");
}

void LEDAnimation::Reset()
{
    curFrame = 0;
    nextFrame = 1;
}


bool LEDAnimation::NextAnimationStep(int64_t timeAlive, sk *driver) 
{
    //Serial.printf("Made it to LEDAnimation's NextAnimationStep with %d TA\n", timeAlive);
    //Serial.printf("curFrame = %d\n", curFrame);
    //Serial.println("curFrame Values");
    //animation.at(curFrame).DebugFrame();
    //Serial.printf("nextFrame = %d\n", nextFrame);
    //Serial.println("nextFrame Values");
    //animation.at(nextFrame).DebugFrame();
    bool ret = true;
    if(timeAlive >= animation.at(curFrame).GetTTL())
    {
        //Serial.println("SWITCHING FRAMES");
        //Serial.printf("Frame switch is %d ms off of desired\n", timeAlive - animation.at(curFrame).GetTTL());
        ret = false;
        PickNextFrame();
        timeAlive = 0;
    }
    animation.at(curFrame).ShowColorsAt(timeAlive, animation.at(nextFrame).GetLEDValues(), driver);
    return ret;
}


bool LEDAnimation::PickNextFrame() 
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

bool LEDAnimation::restartEnding() 
{
    curFrame = nextFrame;
    if((nextFrame + stepper) < animation.size()) 
    {
        nextFrame = nextFrame + stepper;
    }
    else
    {
        nextFrame = 0;
    }
    return true;
}

bool LEDAnimation::reverseEnding() 
{
    curFrame = nextFrame;
    if((nextFrame + stepper) >= 0 && (nextFrame + stepper) < animation.size()) 
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

bool LEDAnimation::randomEnding()
{
    curFrame = nextFrame;
    nextFrame = rand() % animation.size();
}

void LEDAnimation::DebugFrame(int frameNum)
{
    if(animation.size() > frameNum)
    {
        animation.at(frameNum).DebugFrame();
    }
    else
        Serial.printf("Animation does not have frame at index %d\n", frameNum);
}