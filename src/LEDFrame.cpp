#include "LEDFrame.h"   
LEDFrame::LEDFrame(string frameInfo, sk* driver) 
{
    // Serial.println("LEDFrame cconstructor Starting");
    // Serial.printf("LEDFrame consturctor given string\n");
    // Serial.println(frameInfo.c_str());
    this->driver = driver;
    stringstream s_stream(frameInfo);
    vector<string> temp;
    while(s_stream.good()) //breaks string at commas
    {
        string substr;
        getline(s_stream, substr, ',');
        temp.push_back(substr);
    }
    TTL = StringToTime(temp.at(temp.size() - 1));
    temp.pop_back();
    for(int i = 0; i < temp.size(); i++)
    {
//        Serial.print(temp.at(i).c_str()); Serial.print(", ");
        LEDValues.push_back(StringToColor(temp.at(i).c_str()));
    }
    //Serial.println("LEDFrame constructor finsihed");
}

/**
 * @brief Interpols colors for all LEDs, sends resulting values to the driver and displays the new frame.
 * 
 * @param timeAlive Time since start of this frame
 * @param nextFrame 
 * @param brightness 0-100
 */
void LEDFrame::ShowColorsAt(int64_t curtimeAlive, vector<uint32_t> nextFrame) 
{
    float percentage = float(curtimeAlive) / TTL;
    ShowColorsAt(percentage, nextFrame);
}

/**
 * @brief Interpols colors for all LEDs, sends resulting values to the driver and displays the new frame.
 * 
 * @param percentage 
 * @param nextFrame 
 * @param brightness 0-100
 */
void LEDFrame::ShowColorsAt(float percentage, vector<uint32_t> nextFrame)
{
    //Serial.printf("ShowColorsAt entered with %f %\n", percentage);
    vector<uint32_t> displayVector = InterpolFramePercentage(percentage, nextFrame);
    //Serial.printf("Starting writing values to sk driver\n");
    for(int i = 0; i < displayVector.size(); i++)
        driver->color32(i, displayVector.at(i));
    //Serial.printf("Finished writing values, now starting to show colors\n");
    driver->show();
}

/**
 * @brief Interpol from this frame's LED values to another frame's LED values
 * 
 * @param timeAlive Time since start of this frame
 * @param nextFrame 
 * @return vector<uint32_t> 
 */
vector<uint32_t> LEDFrame::InterpolFrame(time_t curtimeAlive, vector<uint32_t> nextFrame) 
{
    float percentage = float(curtimeAlive) / TTL;
    return InterpolFramePercentage(percentage, nextFrame);
}

/**
 * @brief Interpol from this frame's LED values to another frame's LED values
 * 
 * @param percentage percent of time from start of frame til end of frame (start time + TTL)
 * @param nextFrame 
 * @return vector<uint32_t> 
 */
vector<uint32_t> LEDFrame::InterpolFramePercentage(float percentage, vector<uint32_t> nextFrame) 
{
    //Serial.printf("InterpolFramePercentage entered %f %\n", percentage);
    vector<uint32_t> retVector;
    for(int i = 0; i < LEDValues.size(); i++) 
        retVector.push_back(InterpolColors(percentage, LEDValues.at(i), nextFrame.at(i)));
    //Serial.printf("Next colors calculated\n");
    //DebugFrame(retVector);
    return retVector;
}

/**
 * @brief Gets intermetiate color at a certence precentage from curColor to nextColor
 * 
 * @param percentage 
 * @param curColor 
 * @param nextColor 
 * @return uint32_t Interpolled Color
 */
uint32_t LEDFrame::InterpolColors(float percentage, uint32_t curColor, uint32_t nextColor) 
{
//    Serial.printf("Got to interpoling colors with %f %\n", percentage);
    uint8_t *curR, *curG, *curB, *curW;
    uint8_t *nextR, *nextG, *nextB, *nextW;
    driver->breakcolor(curColor, curR, curG, curB, curW);
    driver->breakcolor(nextColor, nextR, nextG, nextB, nextW);
    uint8_t resultR = *curR + (*nextR - *curR) * percentage;
    uint8_t resultG = *curG + (*nextG - *curG) * percentage;
    uint8_t resultB = *curB + (*nextB - *curB) * percentage;
    uint8_t resultW = *curW + (*nextW - *curW) * percentage;
    return driver->getcolor(resultR, resultG, resultB, resultW);
}

void LEDFrame::DebugFrame()
{
    Serial.println("hello?");
    Serial.printf("TTL: %d\n", TTL);
    for(int i = 0; i < LEDValues.size(); i++)
    {
        Serial.printf("%s, ",IntToString(LEDValues.at(i)).c_str());
    }
    Serial.print("\n");
}

void LEDFrame::DebugFrame(vector<uint32_t> frame)
{
    Serial.printf("LEDFrame's driver address: %s\n", AdrToString(driver));
    Serial.printf("TTL: %d\n", TTL);
    for(int i = 0; i < frame.size(); i++)
    {
        Serial.printf("%s, ",IntToString(frame.at(i)).c_str());
    }
    Serial.print("\n");
}