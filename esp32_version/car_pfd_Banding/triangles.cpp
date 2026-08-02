// triangles.cpp — updated
#include "triangles.h"

void drawTriangles(TFT_eSPI &tft, int horizonLeft, int horizonWidth, int yOffset, int bandHeight) {
    // Each triangle's true absolute Y position on the full 320px screen.
    // We only draw it if that position actually falls within this band's
    // Y range (yOffset to yOffset+bandHeight) - otherwise skip entirely,
    // and shift its coordinates down by yOffset so it lands correctly
    // on this band's small sprite.

    // triangle 1 (pointing up), absolute cy = 60
    {
        int absCy = 60;
        int triBase = 20, triHeight = 15;
        if (absCy - triHeight >= yOffset && absCy <= yOffset + bandHeight) {
            int cx = horizonLeft + horizonWidth / 2;
            int cy = absCy - yOffset;
            int p1x = cx - triBase/2, p1y = cy + triHeight;
            int p2x = cx + triBase/2, p2y = cy + triHeight;
            int p3x = cx,             p3y = cy;
            tft.fillTriangle(p1x, p1y, p2x, p2y, p3x, p3y, TFT_BLACK);
            tft.drawTriangle(p1x, p1y, p2x, p2y, p3x, p3y, TFT_WHITE);
        }
    }

    // triangle 2 (pointing down, white fill), absolute cy = 40
    {
        int absCy = 40;
        int triBase = 20, triHeight = 15;
        if (absCy >= yOffset && absCy + triHeight <= yOffset + bandHeight) {
            int cx = horizonLeft + horizonWidth / 2;
            int cy = absCy - yOffset;
            int p1x = cx - triBase/2, p1y = cy;
            int p2x = cx + triBase/2, p2y = cy;
            int p3x = cx,             p3y = cy + triHeight;
            tft.fillTriangle(p1x, p1y, p2x, p2y, p3x, p3y, TFT_WHITE);
            tft.drawTriangle(p1x, p1y, p2x, p2y, p3x, p3y, TFT_WHITE);
        }
    }

    // triangle 3 (pointing down, black fill), absolute cy = 285
    {
        int absCy = 285;
        int triBase = 20, triHeight = 15;
        if (absCy >= yOffset && absCy + triHeight <= yOffset + bandHeight) {
            int cx = horizonLeft + horizonWidth / 2;
            int cy = absCy - yOffset;
            int p1x = cx - triBase/2, p1y = cy;
            int p2x = cx + triBase/2, p2y = cy;
            int p3x = cx,             p3y = cy + triHeight;
            tft.fillTriangle(p1x, p1y, p2x, p2y, p3x, p3y, TFT_BLACK);
            tft.drawTriangle(p1x, p1y, p2x, p2y, p3x, p3y, TFT_WHITE);
        }
    }
}