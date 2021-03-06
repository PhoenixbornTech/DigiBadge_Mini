//TFT controls go here

#define minBRT 25

void startTFT() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST7735_BLACK);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(false);
  tft.setTextSize(1);
  pinMode(BKLT, OUTPUT);
  setLight(bright);
  drawDevInfo(); //Move to after backlight, for backlight-fixing things.
}

void setLight(byte brt) {
  if (brt > 100) {
    //Invalid brightness. Set to a default value.
    brt = 50;
    bright = 50; //We only ever pass Bright to this, so that means a borked Bright setting.
  }
  brt = map(brt, 0, 100, 25, 255); //Set a floor to brightness, so we can always see something.
  analogWrite(6, brt);
  return;
}

void drawLowBat(byte x, byte y) {
  //Draws a low battery symbol with top-left at X,Y.
  //This is designed to be a bit intrusive
  //However, it IS designed to be a bit out of the way so it won't cover up a badge.
  //It fills a black background and adds the battery on top of that
  //To make the symbol visible on all badges and images.
  tft.fillRect(x, y, x + 22, y + 11, ST7735_BLACK);
  tft.fillRect(x + 2, y + 2, x + 16, y + 7, ST7735_RED);
  tft.fillRect(x + 18, y + 4, x + 2, y + 3, ST7735_RED);
}

void drawPlayIcon(byte X, byte Y) {
  //Draws a "Play" symbol to show it's in slideshow mode
  //As with drawLowBat, top/left is at X,Y
  //Originally displayed: X 146, Y 2
  tft.fillTriangle(X, Y, X+11, Y+11, X, Y+22, ST7735_BLACK);
  tft.fillTriangle(X+1, Y+3, X+9, Y+11, X+1, Y+19, ST7735_WHITE);
}

