#ifndef CURVED_SCALES_H
#define CURVED_SCALES_H
#include <TFT_eSPI.h>
void drawRollMarks(TFT_eSPI &tft, int horizonLeft, int horizonTop, int horizonWidth, int horizonHeight, uint16_t colour = TFT_WHITE);
void drawHeadingMarks(TFT_eSPI &tft, int circleCx, int circleCy, int circleRadius, uint16_t colour = TFT_WHITE);
#endif