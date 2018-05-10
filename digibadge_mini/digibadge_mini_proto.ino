//DigiBadge Mini
//TODO list:
//-Backlight Control
//-TFT controls
//-Flash startup

//Libraries for TFT, SPI, SD, and flash.
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include "LowPower.h"
#include <SPIFlash.h>
#include <SPI.h>
#include <SD.h>

//Defines.
//TFT CS is 10, DC is 9. TFT reset is tied to board reset.
#define BKLT 6 //Display backlight pin.
#define BRIGHT 200 //Display brightness
#define SDCS 7 //Chip Select for SD card
#define SDCD 8 //Card Detect line for SD card
#define FLCS 5 //Chip Select for Flash memory.
#define BAUD 74880 //Baud rate for serial communication.
                   //I like 74880 because it's what ESP modules use for their debug.
                   //And I can keep everything on one setting.
#define B0 2 //Button 0, or far left when viewing screen
#define B1 3 //Button 1, or center button
#define B2 4 //Button 2, or far right/next to SD card
#define BWK 0 //Wake on Interrupt 0/Button 0 press. Button 1 is also an option.
#define LowV 2800 //Low Voltage warning, in millivolts
#define CritV 2700 //Critical Voltage. Shut down below this.
#define LLEN 125 //Milliseconds the loop waits for.
#define VLEN 16 //Number of cycles before checking battery. 16 cycles at 125ms is about once every two seconds..
#define CLEN 40 //Cycle length. How many times the loop runs before changing the slideshow.
//#define DEBUG //Uncomment for debugging. And by debugging I mean "Serial" 
              //Be warned! It takes a bit of storage space and variable memory.
              //Current check is ~6% of Program Storage Space and ~6% of dynamic memory.
#define cver "v1.0" //Code Version.

//Set up TFT and Flash
Adafruit_ST7735 tft = Adafruit_ST7735(10, 9, -1);
SPIFlash flash(FLCS);

//Prototypes.

//Variables.
int x = 0;
byte md = 0; //Mode: 0 = Badge, 1 = Slideshow, 2 = Static Image, 3 = Flags
byte badge = 0; //Current badge. 0 = Yellow, 1 = Red, 2 = Green
byte flag = 0; //Current flag. 0 = LGBT, 1 = Bi, 2 = Trans, 3 = Pan, 4 = Ace
byte bobs = 0; //Byte that stores bools as bits.
                //Bits 0, 1, and 2 are the respective Buttons.
                //Bit 3 is "SD Present" - For returning the status of the SD card detector.
                //Bit 4 is "Reset Timer" - Generally, when we manually change the image in a slideshow.
                //Bit 5 is "New Display" - Changing badge, changing image, etc
                //Bit 6 is "Low Battery"
                //Bit 7 is "Just Woke"
byte oldbobs = 0; //Storing previous buttons, to check against.
unsigned int imgnum = 0; //How many images we have.
unsigned int imgcur = 0; //Current image we are displaying.
unsigned int imgprev = 0; //For randomly determining image.

void setup() {
  pinMode(B0, INPUT_PULLUP);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(SDCD, INPUT_PULLUP);
  #ifdef DEBUG
    Serial.begin(BAUD);
    Serial.println(F("DigiBadge Mini"));
    Serial.print(F("Software "));
    Serial.println(F(cver));
    Serial.println(F("Debug Engabled"));
  #endif
  startTFT();
  startSD();
  startFlash();
  delay(2500); //Pause so we can actually SEE the screen.
  updateScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  x += 1;
  if (bitRead(bobs, 7)) {
    //We just woke up, so remove the interrupt and turn the screen back on.
    #ifdef DEBUG
      Serial.println(F("Waking")); 
    #endif
    detachInterrupt(BWK); //Remove the interrupt.
    digitalWrite(6, HIGH);
    bitClear(bobs,7);
    x = 0; //Reset our counter.
    //GetBattery is deliberately placed after we turn back on.
    //This allows for the device to show something, although briefly.
    getBattery();
    delay(250); //Give us time to let go of buttons.
  }
  //Get our buttons. changed.
  getButtons();
  runButtons();
  //Next image in slideshow.
  if ((x % VLEN) == 0){
    //Check voltage.
    getBattery();
  }
  if (x > CLEN){
    if (md == 1){
      //Change image if we're in slideshow mode.
      imgcur = randBMP();
      bitSet(bobs,5);
    }
    getBattery();
    x = 0;
  }
  //Update our screen if we have to.
  if (bitRead(bobs,5)){
    updateScreen();
    //Now that we've updated it, reset it.
    bitClear(bobs, 5);
  }
  if (bitRead(bobs, 4)){
    //We're resetting our timer.
    x = 0;
    bitClear(bobs, 4);
  }
  delay(LLEN);
}

void updateScreen(){
  if (md > 4){
    md = 0; //Just to be sure.
  }
  if (md == 0){
    //Badge Mode.
    drawBadge(badge);
  }
  else if ((md == 1) or (md == 2)){
    dispBMP(imgcur);
  }
  else if (md == 3){
    drawFlag(flag);
  }
  /*else if (md == 4){
    drawMenu(menu, sel);
  }*/
}

void getButtons() {
  //Uses the first three bits of byte "bobs" as bools for buttons.
  //Serial comments left out because they're super spammy.
  //#ifdef DEBUG
  //  Serial.println(F("Gathering buttons."));
  //#endif
  oldbobs = bobs; //Store the previous iterations.  
  //Button 0. Left side facing screen.
  if (digitalRead(B0)){
    bitSet(bobs, 0);
  }
  else {
    bitClear(bobs, 0);
  }
  //Button 1. Center.
  if (digitalRead(B1)){
    bitSet(bobs, 1);
  }
  else {
    bitClear(bobs, 1);
  }
  //Button 2. Next to SD card.
  if (digitalRead(B2)){
    bitSet(bobs, 2);
  }
  else {
    bitClear(bobs, 2);
  }
  //SD Card Detect
  if (!digitalRead(SDCD)){
    //We have an SD card.
    bitSet(bobs, 3);
    if (bitRead(bobs, 3) and (!bitRead(oldbobs, 3))){
      //We now have an SD card, when we didn't before.
      //Reset the device.
      forceRST();
    }
  }
  else {
    bitClear(bobs, 3);
    if bitRead(oldbobs, 3) {
      //SD card removed while running.
      //We can't use images now.
      imgnum = 0;
      imgcur = 0;
    }
  }
}

void getBattery(){
  //Retrieves the voltage of the batteries.
  //Using method prodivded at http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
  //Read the 1.1V internal reference against AVcc
  // Set the reference to VCC and measure the difference between the 1.1v and reference.
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); //Allow Vref to settle.
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long mV = (high << 8) | low;
  mV = 1125300L / mV; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  //#ifdef DEBUG
  //  Serial.print(F("Battery voltage is "));
  //  Serial.print(mV);
  //  Serial.println(F(" millivolts"));
  //#endif
  if (mV <= LowV){
    //We have low voltage.
    //Batteries are still fairly good, but backlight power starts waning quite fast.
    bitSet(bobs, 6);
    #ifdef DEBUG
      Serial.println(F("WARNING: Low batteries!"));
    #endif
  }
  else if ((mV > (LowV + 250)) and bitRead(bobs, 6)){
    //If "Low Battery" bit is set, and we are now higher than the Low Voltage, clear the low battery alert.
    //A small cushion is added to ensure we don't get a set/unset with voltage fluctuations.
    //Realistically speaking, we won't need this, but there's a chance we'll go to, say, external power.
    bitClear(bobs, 6);
  }
  if (mV <= CritV){
    //We turn off at this point.
    //As we check the battery voltage fairly often, we can just shut off.
    #ifdef DEBUG
      Serial.println(F("Battery critical, shutting down."));
    #endif
    digitalWrite(6, LOW);
    attachInterrupt(BWK, wakeUp, LOW);
    delay(300);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }
}

void runButtons(){
  //NOTE: WILL NEED RE-DOING FOR MENU SYSTEM.
  //Menu system will be:
  //In mode: Left/Right change badge/flag/etc
  //Center enters menu.
  //Menu then... menus.
  //Button 0: Power on/off
  if (!bitRead(bobs, 0)){
    napTime();
  }
  //Button 1: Next mode
  if (!bitRead(bobs, 1)){
    bitSet(bobs, 5); //We're changing things, so we'll need a screen update.
    md += 1;
    if ((md == 1) and (imgnum == 1)){
      //With only one image, we can't really do slideshow mode.
      md = 2;
    }
    if ((imgnum < 1) and ((md == 1) or (md == 2))){
      //No SD card, or no images, so skip images.
      md = 3;
    }
    if (md > 3){
      md = 0;
    }
  }
  //Button 2: Next image/badge 
  if (!bitRead(bobs, 2)){
    bitSet(bobs, 5); //We're changing things, so we'll need a screen update.
    if (md == 0){
      //Badge mode. Previous badge.
      badge += 1;
      if (badge > 2){
        badge = 0;
      }
    }
    else if ((md == 1) or (md == 2)){
      //Slideshow or Image mode. Go to the previous image.
      imgcur += 1;
      bitSet(bobs, 4);
      if ((imgcur > imgnum) or (imgcur == 0)){
        //We're too high on our count..
        imgcur = 1;
      }
    }
    else if (md == 3){
      //Flag mode.
      flag += 1;
      if (flag > 4){
        //Wrapping around, like the others.
        flag = 0;
      }
    }
  }
  //Save our settings on a button press.
  if (!bitRead(bobs, 0) or !bitRead(bobs, 1) or !bitRead(bobs, 2)){
    x = 0; //Reset our timer so we don't change too quickly.
    saveSettings();
  }
}

