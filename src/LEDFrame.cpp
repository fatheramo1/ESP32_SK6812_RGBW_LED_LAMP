#include "LEDFrame.h"   
LEDFrame::LEDFrame(sk drive) 
{
    TTL = 0;
    this->driver = driver;
}

/**
 * @brief Interpols colors for all LEDs, sends resulting values to the driver and displays the new frame.
 * 
 * @param timeAlive Time since start of this frame
 * @param nextFrame 
 * @param brightness 0-100
 */
void LEDFrame::ShowColorsAt(int timeAlive, vector<uint32_t> nextFrame, int brightness) 
{
    float percentage = float(timeAlive) / TTL;
    ShowColorsAt(percentage, nextFrame, brightness);
}

/**
 * @brief Interpols colors for all LEDs, sends resulting values to the driver and displays the new frame.
 * 
 * @param percentage 
 * @param nextFrame 
 * @param brightness 0-100
 */
void LEDFrame::ShowColorsAt(float percentage, vector<uint32_t> nextFrame, int brightness)
{
    vector<uint32_t> displayVector = InterpolFramePercentage(percentage, nextFrame);
    for(int i = 0; i < displayVector.size(); i++)
        driver.color32(i, displayVector.at(i), brightness);
    driver.show();
}

/**
 * @brief Interpol from this frame's LED values to another frame's LED values
 * 
 * @param timeAlive Time since start of this frame
 * @param nextFrame 
 * @return vector<uint32_t> 
 */
vector<uint32_t> LEDFrame::InterpolFrame(int timeAlive, vector<uint32_t> nextFrame) 
{
    float percentage = float(timeAlive) / TTL;
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
    vector<uint32_t> retVector;
    for(int i = 0; i < LEDValues.size(); i++) 
        retVector.at(i) = InterpolColors(percentage, LEDValues.at(i), nextFrame.at(i));
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
    uint8_t *curR, *curG, *curB, *curW;
    uint8_t *nextR, *nextG, *nextB, *nextW;
    driver.breakcolor(curColor, curR, curG, curB, curW);
    driver.breakcolor(nextColor, nextR, nextG, nextB, nextW);
    uint8_t resultR = *curR + (*nextR - *curR) * percentage;
    uint8_t resultG = *curG + (*nextG - *curG) * percentage;
    uint8_t resultB = *curB + (*nextB - *curB) * percentage;
    uint8_t resultW = *curW + (*nextW - *curW) * percentage;
    return driver.getcolor(resultR, resultG, resultB, resultW);
}
