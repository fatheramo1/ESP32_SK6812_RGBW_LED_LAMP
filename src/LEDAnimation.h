#include <sk.h>
#include <Arduino.h>
#include <stdlib.h>
#include <vector>
#include <LEDFrame.h>

using namespace std;

class LEDAnimation
{
    /**
    * @brief Used to decide what to do when an animation reaches the end of a frame
    * 
    */
    public: enum EndingType {
    REVERSE,
    RESTART,
    RANDOM
    };

private:
    vector<LEDFrame> animation;
    int curFrame; //pointer to current frame to display within the animation
    int nextFrame;
    int stepper; //used to figure out how to increment nextFrame
    int animationLen;
    sk* driver;
    EndingType frameEnding;

private: 
    EndingType GetEndingType(string ending);
    bool PickNextFrame();
    bool restartEnding();
    bool reverseEnding();
    bool randomEnding();
    void ShowFrame() {driver->show();}
public:
    LEDAnimation(int LEDCount, int animationLen, int stepper, string ending, vector<string> frames, sk *driver);
    bool NextAnimationStep(int64_t timeAlive);
    void DebugFrame(int frameNum);
};