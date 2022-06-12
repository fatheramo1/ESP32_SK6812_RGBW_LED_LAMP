#include "LEDAnimationDriver.h"

LEDAnimationDriver::LEDAnimationDriver(string filePath, int pin)
{
    Serial.println("LEDDriver constructor entered");
    driver = new sk();
    this->pin = pin;
    animations.push_back(readAnimation(filePath));
    driver->begin(pin, LEDCount);
    curAnimation = 0;
    frameTimeAlive = 0;
    speedMod = 1.0;
}

void LEDAnimationDriver::ClearAnimation()
{
    driver->clear();
    frameTimeAlive = 0;
    animations.at(curAnimation).Reset();
}



void LEDAnimationDriver::NextAnimation(int64_t curTime)
{
    frameTimeAlive = 0;
    lastCalledTime = curTime;
    if(curAnimation + 1 >= animations.size())
        curAnimation = 0;
    else
        curAnimation++;
}

bool LEDAnimationDriver::NextAnimationStep(int64_t curTime) 
{
    frameTimeAlive = frameTimeAlive + (curTime - lastCalledTime) * speedMod;
    lastCalledTime = curTime;
    //Serial.printf("Going to LEDAnimations NextAnimationStep with TimeAlive = %d \n", frameTimeAlive);
    if(!animations.at(curAnimation).NextAnimationStep(frameTimeAlive, driver))   //next frame started
        frameTimeAlive = 0;
    return true;
}

void LEDAnimationDriver::AddAnimation(string filePath) 
{
    animations.push_back(readAnimation(filePath));
}

LEDAnimation LEDAnimationDriver::readAnimation(string filePath)
{
    Serial.println("ReadAnimation started");
    if(!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS couldn't begin :(");
    }

    //TODO: Figure out why SPIFFS could not open a file from a string object.
    File file = SPIFFS.open(filePath.c_str());
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
    // Serial.println(LEDCount);
    // Serial.println(stepper);
    // Serial.println(frameEnding.c_str());
    while(file.available())
    {
    //    Serial.println("how many times will this print?");
        frames.push_back(file.readStringUntil('\n').c_str());
    }
    //Serial.println("Did we make it through the for loop?");
    LEDAnimation ret = LEDAnimation(stepper, frameEnding, frames, driver);
    Serial.println("WE MADE IT THROUGH ALL OF THE CONSTRUCTORS");
    return ret;
}

void LEDAnimationDriver::DebugFileOutput()
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