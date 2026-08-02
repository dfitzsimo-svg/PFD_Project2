#ifndef PITCH_BUG_H
#define PITCH_BUG_H
#include <TFT_eSPI.h>
void drawPitchBug(TFT_eSPI &tft, int horizonLeft, int horizonTop, int horizonWidth, int horizonHeight,
    float pitchOffset = 0.0, int barLength = 40, int barThick = 8, int legLength = 10, int legThick = 9,
    int squareSize = 10, int gap = 20, uint16_t bgColour = TFT_BLACK, uint16_t outlineColour = TFT_WHITE);
#endif