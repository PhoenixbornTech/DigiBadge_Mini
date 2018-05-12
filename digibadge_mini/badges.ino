void drawBadge(byte b) {
  //Color Communication Badges
  //Valid numbers are... Any (Realistically 0-2)
  //For more information, see
  //https://autisticadvocacy.org/wp-content/uploads/2014/02/ColorCommunicationBadges.pdf
  //
  //Set text size and color.
  tft.setTextSize(3);
  tft.setTextColor(ST7735_BLACK);
  if (b == 1) {
    //Red badge.
    tft.fillScreen(ST7735_RED);
    tft.fillRect(53, 18, 54, 54, ST7735_BLACK);
    tft.fillRect(56, 21, 48, 48, ST7735_WHITE);
    tft.setCursor(54, 85);
    tft.print(F("RED"));
  }
  else if (b == 2) {
    //Green badge
    tft.fillScreen(ST7735_GREEN);
    tft.fillCircle(80, 45, 27, ST7735_BLACK);
    tft.fillCircle(80, 45, 24, ST7735_WHITE);
    tft.setCursor(36, 85);
    tft.print(F("GREEN"));
  }
  else {
    //Yellow badge.
    //Default to this.
    tft.fillScreen(ST7735_YELLOW);
    tft.fillRect(22, 26, 116, 25, ST7735_BLACK);
    tft.fillRect(25, 29, 110, 19, ST7735_WHITE);
    tft.setCursor(28, 85);
    tft.print(F("YELLOW"));
  }
  //Set size back to default.
  tft.setTextSize(1);
  if (bitRead(bobs,6)) {
    //If our battery is low, show the low battery symbol
    drawLowBat(0, 0);
  }
}
