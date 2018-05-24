//Saving and restoring settings, for when power is removed.

void startFlash() {
  flash.begin();
  readSettings();
}

//We need precisely five flash functions: Read byte, write byte, read int, write int, and erase sector.
//Set up some defines for where we store things.
#define mdAddr 1 //Mode: Address 1
#define bdAddr 2 //Badge: Address 2
#define flAddr 3 //Flag: Address 3
#define curAddr 4 //Current image address: 4
#define brAddr 6 //Current brightness address: 6 (Image takes 2 bytes)
#define scyAddr 7 //Slideshow cycle count: 7

//Now we want to read the information.
void readSettings(){
  //Only done on startup.
  //Comes after SD card startup.
  if (!bitRead(bobs, 3) or (imgnum == 0) or (md > 3)) {
    //We have no images to load, or are in Menu mode, or have an erroneous value
    if ((md == 1) or (md == 2)){
      md = 0; //Reset back to Badge mode.
    }
  }
  badge = flash.readByte(bdAddr); //Badge
  if (badge > 2) {
    //Only 3 valid badges. Badges do error check, but with a borked value it'll show the same one twice in a row.
    badge = 0;
  }
  flag = flash.readByte(flAddr); //Flag
  if (flag > 4) {
    //Only 5 valid flags. Same as badges with error checking.
    flag = 0;
  }
  if (imgnum > 0) {
    //Only do these if we have a loaded SD card.
    imgcur = flash.readWord(curAddr); //Current image.
    if ((imgcur > imgnum) or (imgcur == 0)) {
      //We swapped out our SD cards, apparently. Reset the image number.
      //Either that or we loaded up after not having an SD card.
      imgcur = 1;
    }
  }
  bright = flash.readByte(brAddr);
  if ((bright == 0) or (bright >100)){
    bright == 70; //"0" would show nothing. So set it to "Default"
                  //Higher than 100 isn't valid.
  }
  scycles = flash.readByte(scyAddr);
  if ((scycles == 0) or (scycles > 96)){
    scycles = 40; //"0" would continually cycle. Set to default of 5s.
                  //96 is the max default option, so higher than this is an error.
  }
  return;
}

void saveSettings(){
  flash.eraseSector(1); //We need to clear things first.
  flash.writeByte(mdAddr, md); //Save our mode.
  flash.writeByte(bdAddr, badge); //Save our current badge.
  flash.writeByte(flAddr, flag); //Save our current flag.
  if (bitRead(bobs, 3) and (imgnum > 0)) {
    //Save our current image.
    flash.writeWord(curAddr, imgcur);
  }
  else {
    //We have no current image.
    flash.writeWord(curAddr, 0);
  }
  flash.writeByte(brAddr, bright); //Brightness
  flash.writeByte(scyAddr, scycles); //Slideshow Cycles.
  return;
}
