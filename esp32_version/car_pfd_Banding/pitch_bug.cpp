#include "pitch_bug.h"

void drawPitchBug(TFT_eSPI &tft, int horizonLeft, int horizonTop, int horizonWidth, int horizonHeight,
    float pitchOffset, int barLength, int barThick, int legLength, int legThick,
    int squareSize, int gap, uint16_t bgColour, uint16_t outlineColour) {

    float cy = horizonTop + horizonHeight / 2.0 + pitchOffset;
    float cx = horizonLeft + horizonWidth / 2.0;

    float halfSq = squareSize / 2.0;
    int sqX = (int)(cx - halfSq);
    int sqY = (int)(cy - halfSq);

    float leftHEnd = cx - halfSq - gap;
    float leftHStart = leftHEnd - barLength;

    int leftPtsX[6] = {(int)leftHStart, (int)leftHEnd, (int)leftHEnd, (int)(leftHEnd - legThick), (int)(leftHEnd - legThick), (int)leftHStart};
    int leftPtsY[6] = {(int)(cy - barThick/2.0), (int)(cy - barThick/2.0), (int)(cy + barThick/2.0 + legLength), (int)(cy + barThick/2.0 + legLength), (int)(cy + barThick/2.0), (int)(cy + barThick/2.0)};

    float rightHStart = cx + halfSq + gap;
    float rightHEnd = rightHStart + barLength;

    int rightPtsX[6] = {(int)rightHStart, (int)rightHEnd, (int)rightHEnd, (int)(rightHStart + legThick), (int)(rightHStart + legThick), (int)rightHStart};
    int rightPtsY[6] = {(int)(cy - barThick/2.0), (int)(cy - barThick/2.0), (int)(cy + barThick/2.0), (int)(cy + barThick/2.0), (int)(cy + barThick/2.0 + legLength), (int)(cy + barThick/2.0 + legLength)};

    tft.fillRect((int)leftHStart, (int)(cy - barThick/2.0), (int)barLength, (int)barThick, bgColour);
    tft.fillRect((int)(leftHEnd - legThick), (int)(cy + barThick/2.0), (int)legThick, (int)legLength, bgColour);

    tft.fillRect((int)rightHStart, (int)(cy - barThick/2.0), (int)barLength, (int)barThick, bgColour);
    tft.fillRect((int)rightHStart, (int)(cy + barThick/2.0), (int)legThick, (int)legLength, bgColour);

    tft.fillRect(sqX, sqY, squareSize, squareSize, bgColour);

    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        tft.drawLine(leftPtsX[i], leftPtsY[i], leftPtsX[next], leftPtsY[next], outlineColour);
    }
    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        tft.drawLine(rightPtsX[i], rightPtsY[i], rightPtsX[next], rightPtsY[next], outlineColour);
    }
    tft.drawRect(sqX, sqY, squareSize, squareSize, outlineColour);
}