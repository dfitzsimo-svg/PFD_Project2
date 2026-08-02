#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);  // landscape - adjust 0-3 if orientation looks wrong
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 20);
  tft.println("PFD Test");
  tft.setCursor(20, 50);
  tft.println("Screen OK!");
}

void loop() {
}