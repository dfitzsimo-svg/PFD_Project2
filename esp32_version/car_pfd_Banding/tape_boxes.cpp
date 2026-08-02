// tape_boxes.cpp — updated version

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

    uint16_t tapeGrey = tft.color565(100, 100, 100);  // matches car_pfd.ino's GREY

    int displaySpeed = (int)round(currentSpeed);
    if (displaySpeed < 0) displaySpeed = 0;

    int boxX = tapeLeft;
    int boxY = tapeTop + (tapeHeight - boxHeight) / 2;

    tft.fillRect(boxX, boxY, boxWidth, boxHeight, fillColour);
    tft.drawRect(boxX, boxY, boxWidth, boxHeight, outlineColour);

    // Big number: background matches the black box it actually sits on - unchanged
    tft.setTextColor(textColour, fillColour);
    tft.setTextSize(2);
    tft.setTextDatum(MR_DATUM);
    tft.drawNumber(displaySpeed, boxX + boxWidth / 2, boxY + boxHeight / 2);

    tft.setViewport(tapeLeft, tapeTop, tapeWidth, tapeHeight);

    int tapeBottom = tapeTop + tapeHeight;
    int centreY = tapeTop + tapeHeight / 2;
    int maxRows = tapeHeight / (2 * lineSpacing) + 2;

    tft.setTextSize(1);
    tft.setTextDatum(MR_DATUM);
    tft.setTextColor(textColour, tapeGrey);  // CHANGED: background now matches
                                              // the grey tape strip these
                                              // scrolling numbers actually sit on

    for (int i = 1; i <= maxRows; i++) {
        int upVal = displaySpeed + i * step;
        int downVal = displaySpeed - i * step;
        if (downVal < 0) downVal = 0;

        int upY = centreY - i * lineSpacing;
        int downY = centreY + i * lineSpacing;

        if (upY >= tapeTop && upY <= tapeBottom) tft.drawNumber(upVal, tapeWidth - 10, upY - tapeTop);
        if (downY >= tapeTop && downY <= tapeBottom) tft.drawNumber(downVal, tapeWidth - 10, downY - tapeTop);
    }

    tft.resetViewport();
}

void drawAltitudeTapeValues(TFT_eSPI &tft, int tapeLeft, int tapeTop, int tapeWidth, int tapeHeight,
    float currentAltitude, int boxWidth, int boxHeight, int step, int lineSpacing,
    uint16_t textColour, uint16_t fillColour, uint16_t outlineColour) {

    uint16_t tapeGrey = tft.color565(100, 100, 100);

    int displayAlt = (int)round(currentAltitude);
    if (displayAlt < 0) displayAlt = 0;

    int boxX = tapeLeft;
    int boxY = tapeTop + (tapeHeight - boxHeight) / 2;

    tft.fillRect(boxX, boxY, boxWidth, boxHeight, fillColour);
    tft.drawRect(boxX, boxY, boxWidth, boxHeight, outlineColour);

    tft.setTextColor(textColour, fillColour);
    tft.setTextSize(2);
    tft.setTextDatum(MC_DATUM);
    tft.drawNumber(displayAlt, boxX + boxWidth / 2, boxY + boxHeight / 2);

    tft.setViewport(tapeLeft, tapeTop, tapeWidth, tapeHeight);

    int tapeBottom = tapeTop + tapeHeight;
    int centreY = tapeTop + tapeHeight / 2;
    int maxRows = tapeHeight / (2 * lineSpacing) + 2;

    tft.setTextSize(1);
    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(textColour, tapeGrey);  // CHANGED: same fix

    for (int i = 1; i <= maxRows; i++) {
        int upVal = displayAlt + i * step;
        int downVal = displayAlt - i * step;
        if (downVal < 0) downVal = 0;

        int upY = centreY - i * lineSpacing;
        int downY = centreY + i * lineSpacing;

        if (upY >= tapeTop && upY <= tapeBottom) tft.drawNumber(upVal, 20, upY - tapeTop);
        if (downY >= tapeTop && downY <= tapeBottom) tft.drawNumber(downVal, 20, downY - tapeTop);
    }

    tft.resetViewport();
}