#ifndef ATTITUDE_MARKS_H
#define ATTITUDE_MARKS_H
#include <TFT_eSPI.h>
void drawAttitudeMarks(TFT_eSPI &tft, int horizonLeft, int horizonTop, int horizonWidth, int horizonHeight, uint16_t colour = TFT_WHITE);
#endif