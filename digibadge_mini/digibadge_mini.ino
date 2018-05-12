//DigiBadge Mini
//TODO list:
//-Backlight Control
//-TFT controls
//-Flash startup

//Libraries for TFT, SPI, SD, and flash.
#include <Adafruit_GFX.h>     //https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_ST7735.h>  //https://github.com/adafruit/Adafruit-ST7735-Library
#include <LowPower.h>         //https://github.com/rocketscream/Low-Power
#include <SPIFlash.h>         //https://github.com/Marzogh/SPIFlash
#include <SPI.h>
#include <SD.h>

//Defines.
//TFT CS is 10, DC is 9. TFT reset is tied to board reset.
#define BKLT 6 //Display backlight pin.
#define SDCS 7 //Chip Select for SD card
#define SDCD 8 //Card Detect line for SD card
#define FLCS 5 //Chip Select for Flash memory.
#define B0 2 //Button 0, or far left when viewing screen
#define B1 3 //Button 1, or center button
#define B2 4 //Button 2, or far right/next to SD card
#define BWK 0 //Wake on Interrupt 0/Button 0 press. Button 1 is also an option.
#define LowV 2800 //Low Voltage warning, in millivolts
#define CritV 2700 //Critical Voltage. Shut down below this.
#define LLEN 125 //Milliseconds the loop waits for.
#define VLEN 16 //Number of cycles before checking battery. 16 cycles at 125ms is about once every two seconds..

//Set up TFT and Flash
Adafruit_ST7735 tft = Adafruit_ST7735(10, 9, -1);
SPIFlash flash(FLCS);

//Common strings.
#define cver F("v1.2") //Code version.

//Variables.
byte x = 0;
byte scycles = 40; //Number of cycles it waits for slideshow.
byte bright = 10; //Brightness amount, in percent
byte md = 0; //Mode: 0 = Badge, 1 = Slideshow, 2 = Static Image, 3 = Flags
byte oldmd = 0; //Old mode. For when you enter the menu and just want to exit it.
byte badge = 0; //Current badge. 0 = Yellow, 1 = Red, 2 = Green
byte flag = 0; //Current flag. 0 = LGBT, 1 = Bi, 2 = Trans, 3 = Pan, 4 = Ace
byte menu = 0; //Current menu. 0 = main, 1 = slide speed, 2 = brightness, 3 = device info
byte select = 0; //Current menu selection. Line number. 0 = off.
byte oldselect = 0; //Old selection. For quickly re-displaying cursor.
byte bobs = 0; //Byte that stores bools as bits.
                //Bits 0, 1, and 2 are the respective Buttons.
                //Bit 3 is "SD Present" - For returning the status of the SD card detector.
                //Bit 4 is "Reset Timer" - Generally, when we manually change the image in a slideshow.
                //Bit 5 is "New Display" - Changing badge, changing image, etc
                //Bit 6 is "Low Battery"
                //Bit 7 is "Just Woke"
byte oldbobs = 0; //Storing previous buttons, to check against.
uint16_t imgnum = 0; //How many images we have.
uint16_t imgcur = 0; //Current image we are displaying.

void setup() {
  pinMode(B0, INPUT_PULLUP);
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(SDCD, INPUT_PULLUP);
  startFlash();
  startSD();
  startTFT();
  delay(2500); //Pause so we can actually SEE the screen.
  updateScreen();
}

void loop() {
  // put your main code here, to run repeatedly:
  x += 1;
  if (bitRead(bobs, 7)) {
    //We just woke up, so remove the interrupt and turn the screen back on.
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
  if (x > scycles){
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
    bmpDraw(grabBMP(imgcur),0,0);
  }
  else if (md == 3){
    drawFlag(flag);
  }
  else if (md == 4){
    drawMenu(menu, select);
  }
  if (md == 1){
    drawPlayIcon(146,2);
  }
}

void getButtons() {
  //Uses the first three bits of byte "bobs" as bools for buttons.
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
      //Attempt to re-start the SD card.
      startSD();
      bitSet(bobs, 5); //This makes it easy to notice things have loaded. Especially in a menu.
    }
  }
  else {
    bitClear(bobs, 3);
    if bitRead(oldbobs, 3) {
      //SD card removed while running.
      //We can't use images now.
      imgnum = 0;
      imgcur = 0;
      bitSet(bobs, 5); //As with above, makes it more noticeable that things have changed.
    }
  }
}

long getBattery(){
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
  if (mV <= LowV){
    //We have low voltage.
    //Batteries are still fairly good, but backlight power starts waning quite fast.
    bitSet(bobs, 6);
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
    digitalWrite(6, LOW);
    attachInterrupt(BWK, wakeUp, LOW);
    delay(300);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
  }
  return mV;
}

void runButtons(){
  //New, for Menus!
  //Improvements: "else if" instead of "if"
  //Ensures proper button priority.
  //bitSet(bobs, 5); //Menus won't require full screen reset.
  if (!bitRead(bobs, 1)){
    //Button 1 (Center)
    //Most important button!
    bitSet(bobs, 5); //We're changing menus, or modes.
    //If we're in a non-Menu mode, bring us to Menu mode.
    if(md != 4){
      md = 4;
      menu = 0;
      select = 1;
    }
    else if(md == 4){
      //Do the thing. For the menu.
      if (menu == 0){
        if(select == 1){
          md = 0; //Enter Badge Mode.
        }
        else if (select == 2){
          md = 3; //Enter Flag Mode.
        }
        else if ((select == 3) and (imgnum > 0)){
          md = 2; //Enter Image mode, but only if we have images.
        }
        else if ((select == 4) and (imgnum > 1)){
          md = 1; //Enter Slideshow mode, but only if we have images.
          x = 0; //Reset our timer, just in case.
        }
        else if ((select == 4) and (imgnum == 1)){
          md = 2; //Enter image mode for one image.
        }
        else if (select == 5){
          menu = 3; //Device Info
          select = 0;
        }
        else if (select == 6){
          menu = 1; //Slideshow speed
          select = 1;
        }
        else if (select == 7){
          menu = 2; //Brightness.
          select = 1;
        }
        else if (select == 8){
          md = oldmd; //Make sure we wake up in our old mode.
          //saveSettings(); //Save settings. Shouldn't be needed, but we ARE turning off, so...
          napTime();
        }
        if(((select >=1) and (select <= 4)) or (select == 8)){
          //We're selecting a mode, OR shutting down.
          saveSettings();
        }
      }
      else if (menu == 1){
        //Slideshow speed!
        if((select > 0) and (select < 13)){
          //We're selecting a delay.
          scycles = 8 * select; //Yay simple!
          x = 0; //Reset to 0 for slideshow shenanigans.
          select = 1;
          menu = 0;
        }
        else {
          //Exiting this menu.
          select = 1;
          menu = 0;
        }
      }
      else if (menu == 2){
        //Brightness
        if((select > 0) and (select < 11)){
          //We're selecting a brightness.
          bright = 10 * select;
          setLight(bright);
          //So, to easily see results and change them
          //DON'T exit tue current menu.
          //Also don't redraw it.
          bitClear(bobs, 5);
        }
        else {
          //Exit this menu. Save settings.
          select = 1;
          menu = 0;
        }
      }
      else if (menu == 3){
        //Device Info!
        //Simply return to main menu.
        select = 1;
        menu = 0;
      }
    }
  }
  else if (!bitRead(bobs, 0)){
    //Button 0 (Far left when looking at screen)
    if (md == 0){
      //Badge mode. Decrese badge by one.
      badge -= 1;
      if (badge > 2){
        //Only three badges.
        badge = 2; //We're decreasing, so go to the highest.
      }
    }
    else if (md == 1){
      //Slideshow!
      //New random image.
      imgcur = randBMP();
    }
    else if (md == 2){
      //Still image.
      imgcur -= 1; //Reduce by one!
      if (imgcur > imgnum){
        //We've wrapped around.
        imgcur = imgnum;
      }
    }
    else if (md == 3){
      //Flags!
      flag -= 1;
      if (flag > 4){
        //Wrap around
        flag = 4;
      }
    }
    else if (md == 4){
      //Go "Down" in the menu selection.
      oldselect = select;
      if (menu == 0){
        //Main menu. 8 max.
        select -= 1;
        if (select == 0){
          select = 8;
        }
      }
      else if (menu == 1){
        //Slideshow Time Menu. 13 max.
        select -= 1;
        if (select == 0){
          select = 13;
        }
      }
      else if (menu == 2){
        //Brightness. 11 Max.
        select -= 1;
        if (select == 0){
          select = 11;
        }
      }
      drawCursor(select, oldselect);
    }
  }
  else if (!bitRead(bobs, 2)){
    //Button 2 (Far right, next to SD card)
    if (md == 0){
      //Badge mode. Increase badge by one.
      badge += 1;
      if (badge > 2){
        //Only three badges.
        badge = 0; //Increasing, so wrap around.
      }
    }
    else if (md == 1){
      //Slideshow!
      //New random image.
      imgcur = randBMP();
    }
    else if (md == 2){
      //Still image.
      imgcur += 1; //Increase by one!
      if (imgcur > imgnum){
        //We've wrapped around.
        imgcur = 1;
      }
    }
    else if (md == 3){
      //Flags!
      flag += 1;
      if (flag > 4){
        //Wrap around
        flag = 0;
      }
    }
    else if (md == 4){
      //Go "Up" in the menu selection.
      oldselect = select;
      if (menu == 0){
        //Main menu. 8 max.
        select += 1;
        if (select > 8){
          select = 1;
        }
      }
      else if (menu == 1){
        //Slideshow menu. 13 max.
        select += 1;
        if (select > 13){
          select = 1;
        }
      }
      else if (menu == 2){
        //Brightness menu. 11 max.
        select += 1;
        if (select > 11){
          select = 1;
        }
      }
      drawCursor(select, oldselect);
    }
  }
  if ((!bitRead(bobs, 0) or !bitRead(bobs,2)) and (md != 4)){
    //All non-Menu changes require screen update and settings save.
    bitSet(bobs, 5);
    saveSettings();
  }
}

