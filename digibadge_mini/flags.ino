void drawFlag(byte f) {
  //Draws a particular flag.
  //Flags are various sexual Pride flags.
  //Valid numbers are 0-4, but will error check.
  if (f == 1) {
    //Bi Flag
    tft.fillRect(0, 0, 160, 51, 0xD00F);
    tft.fillRect(0, 51, 160, 26, 0x7272);
    tft.fillRect(0, 77, 160, 51, 0x01D5);
  }
  else if (f == 2) {
    //Trans Flag
    tft.fillRect(0, 0, 160, 26, 0x5E7F);
    tft.fillRect(0, 26, 160, 25, 0xF557);
    tft.fillRect(0, 51, 160, 26, ST7735_WHITE);
    tft.fillRect(0, 77, 160, 25, 0xF557);
    tft.fillRect(0, 102, 160, 26, 0x5E7F);
  }
  else if (f == 3) {
    //Pansexual Flag
    tft.fillRect(0, 0, 160, 43, 0x259F);
    tft.fillRect(0, 43, 160, 42, 0xFEC0);
    tft.fillRect(0, 85, 160, 43, 0xF911);
  }
  else if (f == 4) {
    //Asexual Flag
    tft.fillRect(0, 0, 160, 32, ST7735_BLACK);
    tft.fillRect(0, 32, 160, 32, 0xA514);
    tft.fillRect(0, 64, 160, 32, ST7735_WHITE);
    tft.fillRect(0, 96, 160, 32, 0x8010);
  }
  else if (f == 5) {
    //Nonbinary Flag
    tft.fillRect(0, 0, 160, 32, ST7735_YELLOW);
    tft.fillRect(0, 32, 160, 32, ST7735_WHITE);
    tft.fillRect(0, 64, 160, 32, 0x9AD9); //Light Purple
    tft.fillRect(0, 96, 160, 32, 0x2965); //Not quite black
  }
  else if (f == 6) {
    //Agender Flag
    tft.fillRect(0, 0, 160, 19, ST7735_BLACK);
    tft.fillRect(0, 19, 160, 18, 0xBE38); //Light grey
    tft.fillRect(0, 37, 160, 18, ST7735_WHITE);
    tft.fillRect(0, 55, 160, 18, 0xBFB0); //Lime green ish?
    tft.fillRect(0, 73, 160, 18, ST7735_WHITE);
    tft.fillRect(0, 91, 160, 18, 0xBE38); //Same light grey
    tft.fillRect(0, 109, 160, 19, ST7735_BLACK);
  }
  else {
    //LGBT Pride flag
    //Use as default in case we get our numbers borked.
    tft.fillRect(0, 0, 160, 22, 0xD801); //Pride Red
    tft.fillRect(0, 22, 160, 21, 0xFBC1); //Pride Orange
    tft.fillRect(0, 43, 160, 21, 0xFF61); //Pride Yellow
    tft.fillRect(0, 64, 160, 21, 0x0B83); //Pride Green
    tft.fillRect(0, 85, 160, 21, 0x015F); //Pride Roayl Blue
    tft.fillRect(0, 106, 160, 22, 0x600E); //Pride Violet
  }
  if (bitRead(bobs,6)) {
    //If our battery is low, show the low battery symbol
    drawLowBat(0, 0);
  }
}
