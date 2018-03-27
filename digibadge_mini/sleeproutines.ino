//Routines to sleep and wake the device.

void wakeUp()
{
  // Just a handler for the pin interrupt.
  bitSet(bobs, 7);
}

void napTime()
{
  //Put the device to sleep. Turn the screen off, bind the interrupt key, and put the device to sleep.
  #ifdef DEBUG
    Serial.println(F("Sleeping"));
  #endif
  digitalWrite(6, LOW);
  //saveSettings(); //Save settings, in case someone pulls the battery or something.
  attachInterrupt(BWK, wakeUp, LOW);
  delay(300);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
}
