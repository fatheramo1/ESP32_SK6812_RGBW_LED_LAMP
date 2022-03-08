#include <sk.h>
#include <Arduino.h>
#include <vector>

using namespace std;

class LEDFrame
{
    private: time_t TTL; //Time till next frame
    private: vector<uint32_t> LEDValues;
    private: sk driver;
    
public:
LEDFrame(sk drive);
vector<uint32_t> GetLEDValues() { return LEDValues; }
time_t GetTTL() { return TTL; }
void ShowColorsAt(int timeAlive, vector<uint32_t> nextFrame, int brightness);
void ShowColorsAt(float percentage, vector<uint32_t> nextFrame, int brightness);
vector<uint32_t> InterpolFrame(int timeAlive, vector<uint32_t> nextFrame);
vector<uint32_t> InterpolFramePercentage(float percentage, vector<uint32_t> nextFrame);
uint32_t InterpolColors(float percentage, uint32_t curColor, uint32_t nextColor);    
};
