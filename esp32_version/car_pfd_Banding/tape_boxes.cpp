// tape_boxes.cpp — smooth-scrolling version

#include "tape_boxes.h"

void drawTapeBox(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    int boxWidth, int boxHeight, uint16_t fillColour, uint16_t outlineColour) {
    int x = tapeLeft;
    int y = tapeTop + (tapeHeight - boxHeight) / 2;
    tft.fillRect(x, y, boxWidth, boxHeight, fillColour);
    tft.drawRect(x, y, boxWidth, boxHeight, outlineColour);
}

void drawSpeedTapeValues(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    float currentSpeed, int boxWidth, int boxHeight, int step, int lineSpacing,
    uint16_t textColour, uint16_t fillColour, uint16_t outlineColour) {

    uint16_t tapeGrey = tft.color565(100, 100, 100);

    if (currentSpeed < 0) currentSpeed = 0;
    int displaySpeed = (int)round(currentSpeed);

    int boxX = tapeLeft;
    int boxY = tapeTop + (tapeHeight - boxHeight) / 2;

    tft.fillRect(boxX, boxY, boxWidth, boxHeight, fillColour);
    tft.drawRect(boxX, boxY, boxWidth, boxHeight, outlineColour);

    tft.setTextColor(textColour, fillColour);
    tft.setTextSize(2);
    tft.setTextDatum(MR_DATUM);
    tft.drawNumber(displaySpeed, boxX + boxWidth / 2, boxY + boxHeight / 2);

    tft.setViewport(tapeLeft, tapeTop, tapeWidth, tapeHeight);

    // ORIGINAL APPROACH (values derived from the reading, so the whole
    // ladder jumped by one unit every time the reading changed):
    //   int upVal = displaySpeed + i * step;
    //   int downVal = displaySpeed - i * step;
    //   int upY = centreY - i * lineSpacing;
    //
    // NEW APPROACH: tick values are fixed multiples of `step`. The
    // reading decides where those fixed values sit on screen, so the
    // strip slides continuously past the readout box instead of the
    // numbers themselves changing.

    // How many pixels one unit of speed moves the tape.
    float pixelsPerUnit = (float)lineSpacing / (float)step;

    int centreYLocal = tapeHeight / 2;

    // Half the tape's span expressed in speed units, plus one step of
    // margin so ticks scroll in from off-screen rather than popping in.
    float halfRangeUnits = (tapeHeight / 2.0) / pixelsPerUnit + step;

    // First tick value at or below the bottom of the visible range,
    // snapped down to a multiple of step.
    int startVal = (int)(floor((currentSpeed - halfRangeUnits) / step) * step);
    int endVal   = (int)(ceil((currentSpeed + halfRangeUnits) / step) * step);

    tft.setTextSize(1);
    tft.setTextDatum(MR_DATUM);
    tft.setTextColor(textColour, tapeGrey);

    for (int val = startVal; val <= endVal; val += step) {
        if (val < 0) continue;  // no negative speeds

        // Position is driven by the difference between this tick's
        // fixed value and the live reading.
        int yLocal = centreYLocal - (int)((val - currentSpeed) * pixelsPerUnit);

        if (yLocal < 0 || yLocal > tapeHeight) continue;

        // Skip ticks that would collide with the readout box.
        if (abs(yLocal - centreYLocal) < (boxHeight / 2 + 4)) continue;

        tft.drawNumber(val, tapeWidth - 10, yLocal);
    }

    tft.resetViewport();
}

void drawAltitudeTapeValues(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    float currentAltitude, int boxWidth, int boxHeight, int step, int lineSpacing,
    uint16_t textColour, uint16_t fillColour, uint16_t outlineColour) {

    uint16_t tapeGrey = tft.color565(100, 100, 100);

    int displayAlt = (int)round(currentAltitude);

    int boxX = tapeLeft;
    int boxY = tapeTop + (tapeHeight - boxHeight) / 2;

    tft.fillRect(boxX, boxY, boxWidth, boxHeight, fillColour);
    tft.drawRect(boxX, boxY, boxWidth, boxHeight, outlineColour);

    tft.setTextColor(textColour, fillColour);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawNumber(displayAlt, boxX + boxWidth / 2, boxY + boxHeight / 2);

    tft.setViewport(tapeLeft, tapeTop, tapeWidth, tapeHeight);

    // Same smooth-scrolling approach as the speed tape above.
    // Note: altitude is NOT clamped at zero - below sea level is
    // legitimate, and with a mismatched QNH you can easily read
    // negative on perfectly normal ground.
    float pixelsPerUnit = (float)lineSpacing / (float)step;

    int centreYLocal = tapeHeight / 2;
    float halfRangeUnits = (tapeHeight / 2.0) / pixelsPerUnit + step;

    int startVal = (int)(floor((currentAltitude - halfRangeUnits) / step) * step);
    int endVal   = (int)(ceil((currentAltitude + halfRangeUnits) / step) * step);

    tft.setTextSize(1);
    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(textColour, tapeGrey);

    for (int val = startVal; val <= endVal; val += step) {
        int yLocal = centreYLocal - (int)((val - currentAltitude) * pixelsPerUnit);

        if (yLocal < 0 || yLocal > tapeHeight) continue;
        if (abs(yLocal - centreYLocal) < (boxHeight / 2 + 4)) continue;

        tft.drawNumber(val, 20, yLocal);
    }

    tft.resetViewport();
}