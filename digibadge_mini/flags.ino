void drawFlag(int f) {
  //Draws a particular flag.
  //Flags are various sexual Pride flags.
  //Valid numbers are 0-4
  if ((f > 4) or (f < 0)) {
    //Clamp to 0-4
    #ifdef DEBUG
      Serial.println(F("Invalid flag! Defaulting to LGBT"));
    #endif
    f = 0;
  }
  if (f == 0) {
    //LGBT Pride flag
    #ifdef DEBUG
      Serial.println(F("Displaying LGBT Flag"));
    #endif
    //tft.fillRect(0, 0, 160, 1, ST7735_BLACK);
    tft.fillRect(0, 0, 160, 22, 0xD801); //Pride Red
    tft.fillRect(0, 22, 160, 21, 0xFBC1); //Pride Orange
    tft.fillRect(0, 43, 160, 21, 0xFF61); //Pride Yellow
    tft.fillRect(0, 64, 160, 21, 0x0B83); //Pride Green
    tft.fillRect(0, 85, 160, 21, 0x015F); //Pride Roayl Blue
    tft.fillRect(0, 106, 160, 22, 0x600E); //Pride Violet
    //tft.fillRect(0, 128, 160, 1, ST7735_BLACK);
  }
  else if (f == 1) {
    //Bi Flag
    #ifdef DEBUG
      Serial.println(F("Displaying Bi Flag"));
    #endif
    //tft.fillRect(0, 0, 160, 128, ST7735_BLACK);
    tft.fillRect(0, 0, 160, 51, 0xD00F);
    tft.fillRect(0, 51, 160, 26, 0x7272);
    tft.fillRect(0, 77, 160, 51, 0x01D5);
  }
  else if (f == 2) {
    //Trans Flag
    #ifdef DEBUG
      Serial.println(F("Displaying Trans Flag"));
    #endif
    tft.fillRect(0, 0, 160, 26, 0x5E7F);
    tft.fillRect(0, 26, 160, 25, 0xF557);
    tft.fillRect(0, 51, 160, 26, ST7735_WHITE);
    tft.fillRect(0, 77, 160, 25, 0xF557);
    tft.fillRect(0, 102, 160, 26, 0x5E7F);
  }
  else if (f == 3) {
    //Pansexual Flag
    #ifdef DEBUG
      Serial.println(F("Displaying Pan Flag"));
    #endif
    tft.fillRect(0, 0, 160, 43, 0x259F);
    tft.fillRect(0, 43, 160, 42, 0xFEC0);
    tft.fillRect(0, 85, 160, 43, 0xF911);
  }
  else if (f == 4) {
    //Asexual Flag
    #ifdef DEBUG
      Serial.println(F("Displaying Ace Flag"));
    #endif
    tft.fillRect(0, 0, 160, 32, ST7735_BLACK);
    tft.fillRect(0, 32, 160, 32, 0xA514);
    tft.fillRect(0, 64, 160, 32, ST7735_WHITE);
    tft.fillRect(0, 96, 160, 32, 0x8010);
  }
  if (bitRead(bobs,6)) {
    //If our battery is low, show the low battery symbol
    drawLowBat(0, 0);
  }
}
