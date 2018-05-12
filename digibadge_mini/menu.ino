//Menus.
//Almost fits. Almost.

#define MENU_BGND   ST7735_BLACK //Background color.
#define MENU_TXT    ST7735_GREEN //Color of text. Going for old-school retro.
#define MENU_CUR    ST7735_GREEN //We'll be using background highlighting.
#define MENU_TITLE  ST7735_YELLOW //Titles will be yellow.
#define MENU_WARN   ST7735_RED   //Warning color in menus.
#define MENU_INFO   ST7735_GREEN //If we want non-selectable info to be a different color.
#define MENU_INVAL  0xBDF7 //Invalid items are grey.
#define MENU_SELECT ST7735_YELLOW //Our selection cursor

void drawMenu(byte menu, byte sel){
  //Draws the "menu" with the current "selection"
  //Each menu will be their own sub-function. This will just call the correct function and apply it.
  if (menu == 0){
    drawMainMenu();
  }
  else if (menu == 1){
    drawSlideSpeed();
  }
  else if (menu == 2){
    drawBrightness();
  }
  else if (menu == 3){
    drawDevInfo();
  }
  drawCursor(sel, 0); //We're re-drawing the menu, so don't worry about erasing the old selection.
}

void drawCursor(byte sel, byte oldsel){
  //Displays the current cursor.
  //We use "0" to hide it.
  if (oldsel != 0){
    tft.setCursor(0,9*oldsel);
    tft.setTextColor(MENU_BGND); //Adjust the color.
    tft.print(F("->"));
  }
  if (sel != 0){
    tft.setCursor(0,9*sel); //Set the cursor to the appropriate line.
    tft.setTextColor(MENU_SELECT); //Adjust the color.
    tft.print(F("->"));
  }
}

void drawMainMenu(){
  //Displays the main menu. What a surprise.
  tft.fillScreen(MENU_BGND);
  tft.setCursor(0,0); //Title
  tft.setTextColor(MENU_TITLE);
  tft.print(F("DigiBadge Mini"));
  tft.setCursor(12,9); //Line 1
  tft.setTextColor(MENU_TXT);
  tft.print(F("Color Badges"));
  tft.setCursor(12,18); //Line 2
  tft.print(F("Pride Flags"));
  tft.setCursor(12,45); //Line 5. We skip Image/Slideshow to only have to change colors once. Saves program space.
  tft.print(F("Badge Info"));
  tft.setCursor(12,54); //Line 6
  tft.print(F("Slide Delay"));
  tft.setCursor(12,63); //Line 7
  tft.print(F("Brightness"));
  tft.setCursor(12,72); //Line 8
  tft.print(F("Power Off"));
  tft.setCursor(12,27); //Line 3
  if (imgnum == 0) {
    //We have no images. "Disable" slideshow options.
    tft.setTextColor(MENU_INVAL); //Can go without, since this is first one.
  }
  tft.print(F("Still Image"));
  tft.setCursor(12,36); //Line 4
  tft.print(F("Slideshow"));
}

void drawSlideSpeed(){
  //Displays the options for slideshow speeds.
  tft.fillScreen(MENU_BGND);
  tft.setCursor(0,0);
  tft.setTextColor(MENU_TITLE);
  tft.print(F("Slide Delay"));
  tft.setTextColor(MENU_TXT);
  for(byte i = 1; i <= 12; i++){
    tft.setCursor(12,i*9);
    tft.print(i);
    tft.print(F(" Second"));
  }
  tft.setCursor(12,117); //Last line!
  tft.print(F("Main Menu"));
}

void drawBrightness(){
  //Displays options for brightness
  tft.fillScreen(MENU_BGND);
  tft.setCursor(0,0);
  tft.setTextColor(MENU_TITLE);
  tft.print(F("Brightness"));
  tft.setTextColor(MENU_TXT);
  for(byte x = 1; x <=10; x++){
    tft.setCursor(12,x*9);
    tft.print(x*10);
    tft.print(F("%"));
  }
  tft.setCursor(12,99);
  tft.print(F("Main Menu"));
}

void drawDevInfo(){
  //Displays device info.
  //No selection is visible. Only selection is to exit.
  tft.fillScreen(MENU_BGND);
  tft.setCursor(0,0);
  tft.setTextColor(MENU_TITLE);
  tft.print(F("DigiBadge Mini"));
  tft.setTextColor(MENU_TXT);
  tft.setCursor(0,9);
  tft.print(F("Firmware: "));
  tft.print(cver);
  tft.setCursor(0,18);
  tft.print(F("Battery: "));
  //Do voltage magic here.
  long mV = getBattery();
  if (mV <= LowV){
    tft.setTextColor(ST7735_RED); //We're close to running out of usable power.
  }
  int v = mV/10; //Truncate to 3 digits. IE, 2841 millivolts will become 284.
  float disV = v / 100.0; //Turn into a float that we can display easily. IE, 284 should become 2.84
  tft.print(disV);
  tft.print(F("v"));
  tft.setTextColor(MENU_TXT);
  tft.setCursor(0,27);
  tft.print(F("SD Card: "));
  if (bitRead(bobs, 3)){
    tft.print(F("Yes"));
  }
  else{
    tft.print(F("No"));
  }
  tft.setCursor(0,36);
  tft.print(F("Images: "));
  tft.print(imgnum);
  tft.setCursor(0,45);
  tft.print(F("Slide Delay: "));
  tft.print(scycles / 8);
  tft.print(F("s"));
  tft.setCursor(0,54);
  tft.print(F("Backlight: "));
  tft.print(bright);
  tft.print(F("%"));
  tft.setCursor(0,72);
  tft.print(F("For source visit\nwww.pbtech.biz"));
  //tft.setCursor(0,81);
  //tft.print(F("www.pbtech.biz"));
}

