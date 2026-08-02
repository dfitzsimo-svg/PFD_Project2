#include <TFT_eSPI.h>
#include "triangles.h"
#include "attitude_marks.h"
#include "pitch_bug.h"
#include "tape_boxes.h"
#include "curved_scales.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite band = TFT_eSprite(&tft);

const int WIDTH = 480;
const int HEIGHT = 320;
const int BAND_HEIGHT = 80;
const int NUM_BANDS = HEIGHT / BAND_HEIGHT;  // 4

const int OUTER_GAP = 3;
const int TAPE_WIDTH = 80;
const int HORIZON_LEFT = TAPE_WIDTH + 2 * OUTER_GAP;
const int HORIZON_WIDTH = WIDTH - 2 * HORIZON_LEFT;
const int BLACK_HEIGHT = 40;
const int HORIZON_HEIGHT = HEIGHT - (BLACK_HEIGHT * 2);
const int BLUE_HEIGHT = HORIZON_HEIGHT / 2;
const int MIDDLE = HEIGHT / 2;

unsigned long startMs;

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);

  band.setColorDepth(16);
  bool ok = band.createSprite(WIDTH, BAND_HEIGHT);
  Serial.println(ok ? "Band sprite created OK" : "Band sprite FAILED");

  startMs = millis();
}

void loop() {
  uint16_t colourBlue = band.color565(0, 102, 204);
  uint16_t colourBrown = band.color565(153, 102, 51);
  uint16_t colourGrey = band.color565(100, 100, 100);
  uint16_t colourAppGreen = band.color565(0, 220, 0);

  float elapsedS = (millis() - startMs) / 1000.0;
  float ramp = min(elapsedS / 10.0, 1.0);
  float currentSpeed = 40 * ramp;
  float currentAltitude = 400 * ramp;

  int bottomCircleRadius = HORIZON_WIDTH + 300;
  int bottomCircleCx = HORIZON_LEFT + HORIZON_WIDTH / 2;
  int bottomCircleCy = 895;

  int tapeH = HEIGHT - 2 * OUTER_GAP;
  int leftX = OUTER_GAP;
  int rightX = WIDTH - 100 - OUTER_GAP;

  for (int b = 0; b < NUM_BANDS; b++) {
    int yOffset = b * BAND_HEIGHT;

    band.fillSprite(TFT_BLACK);

    // Everything below draws using (realY - yOffset) so it lands
    // correctly inside this band's small sprite. Content outside
    // 0..BAND_HEIGHT is automatically clipped by the sprite itself.

    band.fillRect(HORIZON_LEFT, BLACK_HEIGHT - yOffset, HORIZON_WIDTH, BLUE_HEIGHT, colourBlue);
    band.fillRect(HORIZON_LEFT, MIDDLE - yOffset, HORIZON_WIDTH, BLUE_HEIGHT, colourBrown);

    band.setTextSize(2);
    band.setTextColor(colourAppGreen, TFT_BLACK);
    band.setTextDatum(ML_DATUM);
    band.drawString("APP", HORIZON_LEFT + 8, BLACK_HEIGHT / 2 - yOffset);
    band.setTextDatum(MR_DATUM);
    band.drawString("STD", HORIZON_LEFT + HORIZON_WIDTH - 8, BLACK_HEIGHT / 2 - yOffset);
    band.setTextDatum(TL_DATUM);

    drawRollMarks(band, HORIZON_LEFT, BLACK_HEIGHT - yOffset, HORIZON_WIDTH, HORIZON_HEIGHT);
    drawAttitudeMarks(band, HORIZON_LEFT, BLACK_HEIGHT - yOffset, HORIZON_WIDTH, HORIZON_HEIGHT);
    drawPitchBug(band, HORIZON_LEFT, BLACK_HEIGHT - yOffset, HORIZON_WIDTH, HORIZON_HEIGHT, 0.0);
    drawTriangles(band, HORIZON_LEFT, HORIZON_WIDTH, yOffset, BAND_HEIGHT);
    //Replaced with above line (triangles in centre): drawTriangles(band, HORIZON_LEFT, HORIZON_WIDTH);
    // NOTE: drawTriangles uses hardcoded absolute Y values internally
    // (60, 40, 285) - see caveat below, this line will need a fix.

    band.fillRect(OUTER_GAP, OUTER_GAP - yOffset, TAPE_WIDTH, HEIGHT - 2 * OUTER_GAP, colourGrey);
    band.fillRect((OUTER_GAP * 2) + TAPE_WIDTH + HORIZON_WIDTH + OUTER_GAP, OUTER_GAP - yOffset,
                 TAPE_WIDTH, HEIGHT - 2 * OUTER_GAP, colourGrey);

    band.fillCircle(bottomCircleCx, bottomCircleCy - yOffset, bottomCircleRadius, colourGrey);

    band.fillRect(0, HEIGHT - OUTER_GAP - yOffset, WIDTH, OUTER_GAP, TFT_BLACK);
    band.fillRect(OUTER_GAP + TAPE_WIDTH, 0 - yOffset, OUTER_GAP, HEIGHT, TFT_BLACK);
    band.fillRect(HORIZON_LEFT + HORIZON_WIDTH, 0 - yOffset, OUTER_GAP, HEIGHT, TFT_BLACK);

    drawHeadingMarks(band, bottomCircleCx, bottomCircleCy - yOffset, bottomCircleRadius);

    drawSpeedTapeValues(band, leftX, OUTER_GAP - yOffset, TAPE_WIDTH, tapeH, currentSpeed);
    drawAltitudeTapeValues(band, rightX, OUTER_GAP - yOffset, TAPE_WIDTH, tapeH, currentAltitude);

    band.pushSprite(0, yOffset);
  }
}