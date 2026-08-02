// attitude_marks.cpp — updated version

#include "attitude_marks.h"

void drawAttitudeMarks(TFT_eSPI &tft, int horizonLeft, int horizonTop, int horizonWidth, int horizonHeight, uint16_t colour) {
    uint16_t skyBlue = tft.color565(0, 102, 204);
    uint16_t groundBrown = tft.color565(153, 102, 51);

    float centreY = horizonTop + horizonHeight / 2.0;
    float pxPerDeg = horizonHeight / 35.0;

    for (int idx = -6; idx <= 6; idx++) {
        float deg = idx * 2.5;
        float y = centreY - deg * pxPerDeg;

        float length;
        if (deg == 0.0) length = horizonWidth;
        else if (fabs(deg) == 10) length = horizonWidth * 0.30;
        else if (fabs(deg) == 5 || fabs(deg) == 15) length = horizonWidth * 0.20;
        else length = horizonWidth * 0.10;

        float x1 = horizonLeft + (horizonWidth - length) / 2.0;
        float x2 = x1 + length;

        tft.drawLine((int)x1, (int)y, (int)x2, (int)y, colour);
        tft.drawLine((int)x1, (int)y + 1, (int)x2, (int)y + 1, colour);

        if (fabs(deg) == 10) {
            // Positive deg (nose up) draws ABOVE centre -> still sky (blue)
            // Negative deg (nose down) draws BELOW centre -> ground (brown)
            // This matches car_pfd.ino's layout: blue on top half, brown on bottom half
            uint16_t bgForLabel = (deg > 0) ? skyBlue : groundBrown;

            tft.setTextColor(colour, bgForLabel);
            tft.setTextSize(1);
            tft.setTextDatum(MR_DATUM);
            tft.drawString("10", (int)x1 - 4, (int)y);
            tft.setTextDatum(TL_DATUM);
        }
    }
}