//Routines to sleep and wake the device.

void(* resetNOW)(void) = 0; //Reset function at address 0

void wakeUp()
{
  // Just a handler for the pin interrupt.
  bitSet(bobs, 7);
}

void napTime()
{
  //Put the device to sleep. Turn the screen off, bind the interrupt key, and put the device to sleep.
  digitalWrite(6, LOW);
  //saveSettings(); //Save settings, in case someone pulls the battery or something.
  attachInterrupt(BWK, wakeUp, LOW);
  delay(300);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
}

void forceRST()
{
  //Resets the board.
  //Turn off the backlight, and blank the screen.
  digitalWrite(6, LOW);
  tft.fillScreen(ST7735_BLACK);
  resetNOW();
}

