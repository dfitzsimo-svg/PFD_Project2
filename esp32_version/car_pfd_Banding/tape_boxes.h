#ifndef TAPE_BOXES_H
#define TAPE_BOXES_H
#include <TFT_eSPI.h>

void drawTapeBox(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    int boxWidth = 50, int boxHeight = 30, uint16_t fillColour = TFT_BLACK, uint16_t outlineColour = TFT_WHITE);

void drawSpeedTapeValues(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    float currentSpeed, int boxWidth = 100, int boxHeight = 50, int step = 10, int lineSpacing = 40,
    uint16_t textColour = TFT_WHITE, uint16_t fillColour = TFT_BLACK, uint16_t outlineColour = TFT_WHITE);

void drawAltitudeTapeValues(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    float currentAltitude, int boxWidth = 100, int boxHeight = 50, int step = 100, int lineSpacing = 40,
    uint16_t textColour = TFT_WHITE, uint16_t fillColour = TFT_BLACK, uint16_t outlineColour = TFT_WHITE);

#endif