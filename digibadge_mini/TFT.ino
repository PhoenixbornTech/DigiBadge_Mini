//TFT controls go here

#define minBRT 25

void startTFT() {
  #ifdef DEBUG
    Serial.println(F("Starting TFT..."));
  #endif
  //tft.init();
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(1);
  /*tft.setCursor(0, 0);
  tft.print(F("DigiBadge Mini Starting..."));
  tft.setCursor(0, 9);
  tft.print(F("Made in 2018 by"));
  tft.setCursor(0, 18);
  tft.print(F("Phoenixborn Technologies"));
  tft.setCursor(0, 27);
  tft.print(F("http://www.pbtech.biz"));
  tft.setCursor(133, 120);
  tft.print(cver);*/
  drawDevInfo();
  pinMode(BKLT, OUTPUT);
  setLight(bright);
  #ifdef DEBUG
    Serial.println(F("TFT Started!"));
  #endif
}

void setLight(byte brt) {
  brt = map(brt, 0, 100, 0, 255);
  int bpct = (brt * 100) / 255; //To avoid flipping between int and float, adjust the brightness up.
  analogWrite(6, brt);
  #ifdef DEBUG
    Serial.print(F("Backlight set to "));
    Serial.print(bpct);
    Serial.println(F("%"));
  #endif
}

void drawLowBat(int x, int y) {
  //Draws a low battery symbol with top-left at X,Y.
  //This is designed to be a bit intrusive
  //However, it IS designed to be a bit out of the way so it won't cover up a badge.
  //It fills a black background and adds the battery on top of that
  //To make the symbol visible on all badges and images.
  tft.fillRect(x, y, x + 22, y + 11, ST7735_BLACK);
  tft.fillRect(x + 2, y + 2, x + 16, y + 7, ST7735_RED);
  tft.fillRect(x + 18, y + 4, x + 2, y + 3, ST7735_RED);
  tft.drawLine(x + 8, y + 10, x + 13, y, ST7735_BLACK);
  tft.drawLine(x + 7, y + 10, x + 12, y, ST7735_BLACK);
}

void drawCritBat() {
  //Fills the screen with a low battery symbol.
  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(15, 39, 117, 50, ST7735_RED);
  tft.fillRect(132, 52, 14, 24, ST7735_RED);
  tft.fillRect(18, 42, 111, 44, ST7735_BLACK);
}

void drawPlayIcon(int X, int Y) {
  //Draws a "Play" symbol to show it's in slideshow mode
  //As with drawLowBat, top/left is at X,Y
  //Originally displayed: X 146, Y 2
  tft.fillTriangle(X, Y, X+11, Y+11, X, Y+22, ST7735_BLACK);
  tft.fillTriangle(X+1, Y+3, X+9, Y+11, X+1, Y+19, ST7735_WHITE);
}

