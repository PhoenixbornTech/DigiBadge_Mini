//Saving and retrieving storage will go here.
//Some issues have shown up with flash memory not working
//And by some issues I mean the chip isn't being seen at all.
/*
#define FPage 1 //Page of Flash memory we use.
#define mdByte 1 //Byte offset (0-255) for which byte is the Mode.
#define bdByte 2 //Byte offset for which badge we are using.
#define flByte 3 //Byte offset for which flag we are using.
#define imgByte 4 //Byte offset for image name.

//flash.eraseSector(page, 0);

//flash.writeByte(page, offset, dataByte) //Writes a byte

//flash.readByte(page, offset) //Returns Byte
//      flash.readStr(page, offset, outputString);
//      flash.writeStr(page, offset, inputString)

void saveSettings(){
  #ifdef DEBUG
    Serial.println(F("Saving settings..."));
  #endif
  String imgname;
  imgname = String(grabBMP(imgcur));
  Serial.println(imgname);
  //Erase the page. 
  flash.eraseSector(FPage, 0);
  //Write our mode.
  flash.writeByte(FPage, mdByte);
  //Write our badge.
  flash.writeByte(FPage, bdByte);
  //Write our flag.
  flash.writeByte(FPage, flByte);
  //Write what image we were on.
  flash.writeStr(FPage, imgByte, imgname);
}

void validSettings(){
  //Validates that the settings we loaded are accurate.
  if(md > 3){
    md = 0;
  }
  if(badge > 2){
    badge = 0;
  }
  if(flag > 4){
    flag = 0;
  }
  if (imgcur > imgnum){
    imgcur = 1;
  }
}

void loadSettings(){
  #ifdef DEBUG
    Serial.println(F("Loading settings..."));
  #endif
  //Load mode.
  md = flash.readByte(FPage, byte(mdByte));
  //Load badge.
  badge = flash.readByte(FPage, byte(bdByte));
  //Load flag.
  flag = flash.readByte(FPage, byte(flByte));
  //Load what image we are on.
  String imgload;
  flash.readStr(FPage, imgByte, imgload);
  char timg[sizeof(imgload)];
  imgload.toCharArray(timg, sizeof(imgload));
  //Find if we have it, and what number it is.
  unsigned int nimg;
  nimg = numBMP(timg);
  if (nimg != 0){
    imgcur = nimg;
  }
}*/

