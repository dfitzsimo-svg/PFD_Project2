#include "curved_scales.h"
#include <math.h>

static void pointOnArc(int cx, int cy, int radius, float angleDeg, int &outX, int &outY) {
    float a = angleDeg * PI / 180.0;
    outX = cx + (int)(radius * sin(a));
    outY = cy - (int)(radius * cos(a));
}

void drawRollMarks(TFT_eSPI &tft, int horizonLeft, int horizonTop, int horizonWidth, int horizonHeight, uint16_t colour) {
    float cx = horizonLeft + horizonWidth / 2.0;
    float apexY = horizonTop;
    float edgeDrop = 18.0;
    float halfSpan = horizonWidth / 2.0;
    float radius = (halfSpan * halfSpan + edgeDrop * edgeDrop) / (2.0 * edgeDrop);
    float cy = apexY + radius + 10;
    float endDeg = asin(halfSpan / radius) * 180.0 / PI;

    float tickFracs[] = {-1.0, -0.75, -0.5, -0.33, -0.16, 0.0, 0.16, 0.33, 0.5, 0.75, 1.0};
    for (int i = 0; i < 11; i++) {
        float f = tickFracs[i];
        float ang = f * endDeg;
        int tickLen = (fabs(f) == 1.0 || fabs(f) == 0.5 || f == 0.0) ? 14 : 9;
        int outerX, outerY, innerX, innerY;
        pointOnArc((int)cx, (int)cy, (int)radius, ang, outerX, outerY);
        pointOnArc((int)cx, (int)cy, (int)(radius - tickLen), ang, innerX, innerY);
        tft.drawLine(outerX, outerY, innerX, innerY, colour);
    }
}

void drawHeadingMarks(TFT_eSPI &tft, int circleCx, int circleCy, int circleRadius, uint16_t colour) {
    int prevX, prevY;
    pointOnArc(circleCx, circleCy, circleRadius, -14, prevX, prevY);
    for (int d = -13; d <= 14; d++) {
        int x, y;
        pointOnArc(circleCx, circleCy, circleRadius, d, x, y);
        tft.drawLine(prevX, prevY, x, y, colour);
        prevX = x;
        prevY = y;
    }
    for (int ang = -12; ang <= 12; ang += 3) {
        int tickLen = (ang % 6 == 0) ? 12 : 7;
        int outerX, outerY, innerX, innerY;
        pointOnArc(circleCx, circleCy, circleRadius, ang, outerX, outerY);
        pointOnArc(circleCx, circleCy, circleRadius - tickLen, ang, innerX, innerY);
        tft.drawLine(outerX, outerY, innerX, innerY, colour);
    }
}