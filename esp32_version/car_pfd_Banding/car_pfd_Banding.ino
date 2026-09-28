#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_MPU6050.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include "triangles.h"
#include "attitude_marks.h"
#include "pitch_bug.h"
#include "tape_boxes.h"
#include "curved_scales.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite centerBand = TFT_eSprite(&tft);

Adafruit_BMP280 bmp;
bool bmpOK = false;
float seaLevelhPa = 1013; //hPa

Adafruit_MPU6050 mpu;
bool mpuOK = false;

// Exponential moving average smoothing for pitch.
// Each new reading contributes only PITCH_SMOOTHING of its value,
// the rest carries over from the previous smoothed value. Lower
// value = smoother but laggier. 0.15 is a reasonable starting point;
// tune it once you see how it feels on the road.
const float PITCH_SMOOTHING = 0.15;
float smoothedPitchDeg = 0.0;
bool pitchInitialised = false;

// IMU read timing - independent of both the frame cap and the
// barometer's 200ms interval. 20ms = 50Hz, fast enough that the
// smoothing has plenty of samples to work with without hammering
// the I2C bus.
unsigned long lastImuReadMs = 0;
const unsigned long IMU_READ_INTERVAL_MS = 20;

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // UART2 - GPIO16 RX, GPIO17 TX

// Track whether we've ever had a valid fix. Once true, we start
// showing GPS speed instead of the ramp. If the fix is later lost,
// gpsSpeedKmh keeps the last known value rather than dropping to 0,
// which would look worse than a slightly stale reading.
bool gpsHasFix = false;
float gpsSpeedKmh = 0.0;


const int WIDTH = 480;
const int HEIGHT = 320;
const int BAND_HEIGHT = 80;

const int OUTER_GAP = 3;
const int TAPE_WIDTH = 80;
const int HORIZON_LEFT = TAPE_WIDTH + 2 * OUTER_GAP;
const int HORIZON_WIDTH = WIDTH - 2 * HORIZON_LEFT;
const int BLACK_HEIGHT = 40;
const int HORIZON_HEIGHT = HEIGHT - (BLACK_HEIGHT * 2);
const int MIDDLE = HEIGHT / 2;

const int WINDOW_TOP = BLACK_HEIGHT;
const int WINDOW_BOTTOM = BLACK_HEIGHT + HORIZON_HEIGHT;

const float PX_PER_DEG = HORIZON_HEIGHT / 35.0;

const int CENTER_BANDS = (WINDOW_BOTTOM - WINDOW_TOP) / BAND_HEIGHT;

uint16_t colourBlue;
uint16_t colourBrown;
uint16_t colourGrey;
uint16_t colourAppGreen;

const int bottomCircleRadius = HORIZON_WIDTH + 300;
const int bottomCircleCx = HORIZON_LEFT + HORIZON_WIDTH / 2;
const int bottomCircleCy = 895;
const int tapeH = HEIGHT - 2 * OUTER_GAP;
const int leftX = OUTER_GAP;
const int rightX = WIDTH - 100 - OUTER_GAP;

const int BOX_WIDTH = 100;
const int BOX_HEIGHT = 50;
const int BOX_Y = OUTER_GAP + (tapeH - BOX_HEIGHT) / 2;

unsigned long startMs;
unsigned long lastSensorReadMs = 0;
const unsigned long SENSOR_READ_INTERVAL_MS = 200;
unsigned long lastFrameMs = 0;
const unsigned long FRAME_INTERVAL_MS = 33;

float currentAltitude = 0.0;
float currentSpeed = 0.0;

int prevDisplaySpeed = -1;
int prevDisplayAlt = -1;

float currentPitchDeg = 0.0;
const float FAKE_PITCH_AMPLITUDE_DEG = 5.0;
const float FAKE_PITCH_PERIOD_S = 6.0;



void drawStaticFrame() {
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(HORIZON_LEFT, WINDOW_TOP, HORIZON_WIDTH,
               HORIZON_HEIGHT / 2, colourBlue);
  tft.fillRect(HORIZON_LEFT, MIDDLE, HORIZON_WIDTH,
               HORIZON_HEIGHT / 2, colourBrown);

  tft.fillRect(OUTER_GAP, OUTER_GAP, TAPE_WIDTH, tapeH, colourGrey);
  tft.fillRect((OUTER_GAP * 2) + TAPE_WIDTH + HORIZON_WIDTH + OUTER_GAP,
               OUTER_GAP, TAPE_WIDTH, tapeH, colourGrey);

  tft.fillCircle(bottomCircleCx, bottomCircleCy, bottomCircleRadius, colourGrey);
  drawHeadingMarks(tft, bottomCircleCx, bottomCircleCy, bottomCircleRadius);

  tft.fillRect(0, 0, WIDTH, OUTER_GAP, TFT_BLACK);
  tft.fillRect(0, HEIGHT - OUTER_GAP, WIDTH, OUTER_GAP, TFT_BLACK);
  tft.fillRect(0, 0, OUTER_GAP, HEIGHT, TFT_BLACK);
  tft.fillRect(OUTER_GAP + TAPE_WIDTH, 0, OUTER_GAP, HEIGHT, TFT_BLACK);
  tft.fillRect(HORIZON_LEFT + HORIZON_WIDTH, 0, OUTER_GAP, HEIGHT, TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextColor(colourAppGreen, TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  tft.drawString("APP", HORIZON_LEFT + 8, BLACK_HEIGHT / 2);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("STD", HORIZON_LEFT + HORIZON_WIDTH - 8, BLACK_HEIGHT / 2);
  tft.setTextDatum(TL_DATUM);

  {
    int cx = HORIZON_LEFT + HORIZON_WIDTH / 2;
    int absCy = 285;
    int triBase = 20, triHeight = 15;
    tft.fillTriangle(cx - triBase / 2, absCy, cx + triBase / 2, absCy,
                     cx, absCy + triHeight, TFT_BLACK);
    tft.drawTriangle(cx - triBase / 2, absCy, cx + triBase / 2, absCy,
                     cx, absCy + triHeight, TFT_WHITE);
  }

  drawSpeedTapeValues(tft, leftX, OUTER_GAP, TAPE_WIDTH, tapeH, currentSpeed);
  drawAltitudeTapeValues(tft, rightX, OUTER_GAP, TAPE_WIDTH, tapeH, currentAltitude);
  prevDisplaySpeed = max(0, (int)round(currentSpeed));
  prevDisplayAlt = max(0, (int)round(currentAltitude));
}

void updateSpeedTape() {
  tft.fillRect(OUTER_GAP, OUTER_GAP, TAPE_WIDTH, tapeH, colourGrey);
  drawSpeedTapeValues(tft, leftX, OUTER_GAP, TAPE_WIDTH, tapeH, currentSpeed);
  tft.fillRect(OUTER_GAP + TAPE_WIDTH, 0, OUTER_GAP, HEIGHT, TFT_BLACK); 
}

void updateAltitudeTape() {
  int altTapeX = (OUTER_GAP * 2) + TAPE_WIDTH + HORIZON_WIDTH + OUTER_GAP;
  tft.fillRect(altTapeX, OUTER_GAP, TAPE_WIDTH, tapeH, colourGrey);
  drawAltitudeTapeValues(tft, rightX, OUTER_GAP, TAPE_WIDTH, tapeH, currentAltitude);
  tft.fillRect(HORIZON_LEFT + HORIZON_WIDTH, 0, OUTER_GAP, HEIGHT, TFT_BLACK); 
}






void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);

  colourBlue     = tft.color565(0, 102, 204);
  colourBrown    = tft.color565(153, 102, 51);
  colourGrey     = tft.color565(100, 100, 100);
  colourAppGreen = tft.color565(0, 220, 0);

  centerBand.setColorDepth(16);
  bool ok = centerBand.createSprite(HORIZON_WIDTH, BAND_HEIGHT);
  Serial.println(ok ? "Center band sprite created OK" : "Center band sprite FAILED");

  Wire.begin();

  bmpOK = bmp.begin(0x76);
  if (!bmpOK) {
    bmpOK = bmp.begin(0x77);
  }

  if (bmpOK) {
    Serial.println("BMP280 found OK");
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_125);
  } else {
    Serial.println("BMP280 NOT found - check wiring. Altitude will read 0.");
  }



  mpuOK = mpu.begin();
  if (mpuOK) {
    Serial.println("MPU6050 found OK");
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  } else {
    Serial.println("MPU6050 NOT found - pitch will stay at 0.");
  }

  if (!mpu.begin()) {
    Serial.println("MPU6050 NOT found - check wiring");
  } else {
    Serial.println("MPU6050 found OK");
  }

  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  Serial.println("GPS serial started on UART2 (RX=16, TX=17)");


  drawStaticFrame();

  startMs = millis();
}

void loop() {
  unsigned long nowMs = millis();

  // ---- Barometer read (200ms) ----
  if (bmpOK && (nowMs - lastSensorReadMs >= SENSOR_READ_INTERVAL_MS)) {
    lastSensorReadMs = nowMs;
    float alt = bmp.readAltitude(seaLevelhPa);
    if (!isnan(alt)) {
      currentAltitude = alt * 3.28084;  // metres -> feet
    }
  }

  // ---- IMU read (20ms / 50Hz) ----
  // Runs before the frame cap so the smoothing filter keeps getting
  // samples even on frames that get skipped.
  if (mpuOK && (nowMs - lastImuReadMs >= IMU_READ_INTERVAL_MS)) {
    lastImuReadMs = nowMs;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Y axis as the tilt axis (rotated 90 deg from the original X).
    float rawPitchDeg = -atan2(a.acceleration.y,
                              sqrt(a.acceleration.x * a.acceleration.x +
                                   a.acceleration.z * a.acceleration.z))
                        * 180.0 / PI;

    if (!pitchInitialised) {
      smoothedPitchDeg = rawPitchDeg;
      pitchInitialised = true;
    } else {
      smoothedPitchDeg = PITCH_SMOOTHING * rawPitchDeg +
                         (1.0 - PITCH_SMOOTHING) * smoothedPitchDeg;
    }
  }

  // GPS parsing - non-blocking. TinyGPSPlus needs to see every byte
  // as it comes in, so we drain the UART buffer every loop iteration.
  // Sentences arrive at 1Hz by default, but individual chars stream
  // in continuously, so this must run often.
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // A complete sentence just parsed. Update our cached values.
      if (gps.speed.isValid()) {
        gpsSpeedKmh = gps.speed.kmph();
        gpsHasFix = true;
      }
    }

  }

  // Diagnostic: print GPS status once a second so we can see whether
  // the module is talking to us at all.
  static unsigned long lastGpsDebugMs = 0;
  if (nowMs - lastGpsDebugMs >= 1000) {
    lastGpsDebugMs = nowMs;
    Serial.print("GPS chars: ");
    Serial.print(gps.charsProcessed());
    Serial.print("   sats: ");
    Serial.print(gps.satellites.value());
    Serial.print("   fix: ");
    Serial.print(gps.location.isValid() ? "YES" : "no");
    Serial.print("   speed valid: ");
    Serial.println(gps.speed.isValid() ? "YES" : "no");
  }

  // ---- Frame cap (~30fps) ----
  if (nowMs - lastFrameMs < FRAME_INTERVAL_MS) return;
  lastFrameMs = nowMs;

  float elapsedS = (nowMs - startMs) / 1000.0;
  //float ramp = min(elapsedS / 10.0, 1.0); For testing speed ramp
  //currentSpeed = 40 * ramp;  // TODO: replace with NEO-6M GPS speed

  // Use GPS speed once we've had a fix. Before the first fix, show 0
  // rather than the old ramp - a stationary reading is more honest
  // than fake test data on a device that's supposed to work now.
  if (gpsHasFix) {
    currentSpeed = gpsSpeedKmh;
  } else {
    currentSpeed = 0.0;
  }

  // ---- Tape updates ----
  // Float comparison with a small threshold, so the tape redraws on
  // sub-unit changes now that it scrolls smoothly. Speed is 8 px per
  // km/h (lineSpacing 40 / step 5), so 0.25 units is ~2px of movement.
  // Altitude is 4 px per metre (40 / 10), so 0.5 units is ~2px.
  if (fabs(currentSpeed - prevDisplaySpeed) >= 0.25) {
    prevDisplaySpeed = currentSpeed;
    updateSpeedTape();
  }

  if (fabs(currentAltitude - prevDisplayAlt) >= 0.5) {
    prevDisplayAlt = currentAltitude;
    updateAltitudeTape();
  }

  // ---- Pitch ----
  // Real IMU data now. Fake sine oscillation kept commented for
  // rendering tests without the sensor attached:
  // currentPitchDeg = FAKE_PITCH_AMPLITUDE_DEG * sin(2.0 * PI * elapsedS / FAKE_PITCH_PERIOD_S);
  currentPitchDeg = smoothedPitchDeg;

  int pitchPixelOffset = (int)(currentPitchDeg * PX_PER_DEG);

  int horizonY = MIDDLE + pitchPixelOffset;
  if (horizonY < WINDOW_TOP) horizonY = WINDOW_TOP;
  if (horizonY > WINDOW_BOTTOM) horizonY = WINDOW_BOTTOM;

  // ---- Attitude window: 3 banded sprite pushes ----
  for (int cb = 0; cb < CENTER_BANDS; cb++) {
    int centerYOffset = WINDOW_TOP + cb * BAND_HEIGHT;

    centerBand.fillSprite(colourBlue);
    centerBand.fillRect(0, horizonY - centerYOffset, HORIZON_WIDTH,
                        WINDOW_BOTTOM - horizonY, colourBrown);

    drawAttitudeMarks(centerBand, 0,
                      BLACK_HEIGHT + pitchPixelOffset - centerYOffset,
                      HORIZON_WIDTH, HORIZON_HEIGHT);

    drawRollMarks(centerBand, 0,
                  BLACK_HEIGHT - centerYOffset,
                  HORIZON_WIDTH, HORIZON_HEIGHT);

    drawPitchBug(centerBand, 0,
                 BLACK_HEIGHT - centerYOffset,
                 HORIZON_WIDTH, HORIZON_HEIGHT, 0.0);

    drawTriangles(centerBand, 0, HORIZON_WIDTH, centerYOffset, BAND_HEIGHT);

    // Readout box edges painted into the sprite so they arrive already
    // correct - no second draw on top, nothing to flicker.
    {
      int boxTopLocal = BOX_Y - centerYOffset;
      int boxBotLocal = BOX_Y + BOX_HEIGHT - centerYOffset;

      if (boxBotLocal > 0 && boxTopLocal < BAND_HEIGHT) {
        int leftOverlapW = (leftX + BOX_WIDTH) - HORIZON_LEFT;
        int rightOverlapX = rightX - HORIZON_LEFT;
        int rightOverlapW = (HORIZON_LEFT + HORIZON_WIDTH) - rightX;

        centerBand.fillRect(0, boxTopLocal, leftOverlapW, BOX_HEIGHT, TFT_BLACK);
        centerBand.drawFastVLine(leftOverlapW - 1, boxTopLocal, BOX_HEIGHT, TFT_WHITE);
        centerBand.drawFastHLine(0, boxTopLocal, leftOverlapW, TFT_WHITE);
        centerBand.drawFastHLine(0, boxBotLocal - 1, leftOverlapW, TFT_WHITE);

        centerBand.fillRect(rightOverlapX, boxTopLocal, rightOverlapW, BOX_HEIGHT, TFT_BLACK);
        centerBand.drawFastVLine(rightOverlapX, boxTopLocal, BOX_HEIGHT, TFT_WHITE);
        centerBand.drawFastHLine(rightOverlapX, boxTopLocal, rightOverlapW, TFT_WHITE);
        centerBand.drawFastHLine(rightOverlapX, boxBotLocal - 1, rightOverlapW, TFT_WHITE);
      }
    }

    centerBand.pushSprite(HORIZON_LEFT, centerYOffset);
  }
}