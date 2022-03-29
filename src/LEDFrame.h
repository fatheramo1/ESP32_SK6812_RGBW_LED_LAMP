#include <sk.h>
#include <Arduino.h>
#include <sstream>
#include <vector>

using namespace std;

class LEDFrame
{
    private: int64_t TTL; //Time till next frame
    private: vector<uint32_t> LEDValues;
    private: sk* driver;
    
public:
LEDFrame(string frameInfo, sk* driver);
vector<uint32_t> GetLEDValues() { return LEDValues; }
time_t GetTTL() { return TTL; }
void ShowColorsAt(int64_t curtimeAlive, vector<uint32_t> nextFrame);
void ShowColorsAt(float percentage, vector<uint32_t> nextFrame);
vector<uint32_t> InterpolFrame(time_t curtimeAlive, vector<uint32_t> nextFrame);
vector<uint32_t> InterpolFramePercentage(float percentage, vector<uint32_t> nextFrame);
uint32_t InterpolColors(float percentage, uint32_t curColor, uint32_t nextColor);

public: void DebugFrame();
public: void DebugFrame(vector<uint32_t> frame);
private: const uint32_t StringToColor(string value) 
{
    try
    {
        stringstream s(value);
        uint32_t ret;
            s >> std::hex >> ret;
        return ret;
    }
    catch (exception e) 
    {
        Serial.println(e.what());
    }
}

private: const int64_t StringToTime(string value) 
{
    stringstream s(value);
    int64_t ret;
        s >> ret;
    return ret;
}

//Doesn't add padded zeros to keep the number in 32 bit form.
private: const string IntToString(int value)
{
    stringstream ret;
    ret << std::hex << value;
    return ret.str();
}

public: const string AdrToString(void * adr) 
{
    stringstream ret;
    ret << adr;
    return ret.str();
}
};
