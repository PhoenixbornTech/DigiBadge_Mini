void startSD(){
  #ifdef DEBUG
    Serial.print("Initializing SD card...");
  #endif
  tft.setCursor(0, 32);
  tft.print(F("Starting SD Card..."));
  tft.setCursor(0, 40);
  if (!SD.begin(SDCS)) {
    #ifdef DEBUG
      Serial.println("failed!");
    #endif
    tft.print(F("No card or load failure."));
    bitClear(bobs, 3);
    return;
  }
  bitSet(bobs, 3);
  tft.print(F("Success!"));
  tft.setCursor(0, 48);
  #ifdef DEBUG
    Serial.println("OK!");
  #endif
  //Begin sorting.
  imgnum = countBMP();
  tft.print(imgnum);
  tft.print(F(" valid images found."));
  if (imgnum > 0) {
    //"Current Image" starts at 1, so make sure we start there if we have images.
    imgcur = 1;
  }
  #ifdef DEBUG
    Serial.print(imgnum);
    Serial.println(F(" Valid BMPs found"));
  #endif
}

unsigned int countBMP(){
  unsigned int bmpcount = 0;
  //"Good" BMP files are moved to the "MiniBMP" folder.
  //All other files and folders are ignored.
  //First, make sure the "MiniBMP" folder exists
  #ifdef DEBUG
    Serial.println(F("Counting files on SD Card"));
  #endif
  //Open directory, then rewind it. Just in case someone hasn't been kind.
  File dir = SD.open("/");
  dir.rewindDirectory();
  char* nameHolder;
  bool check = false;
  //Start checking files.
  File fi = dir.openNextFile();
  while (fi) {
    //Check it.
    if (bmpCheck(fi)){
      //We have a valid, 24-bit BMP file.
      bmpcount += 1;
    }
    //We're done here.
    fi.close();
    delay(5); //Don't go too fast.
    fi = dir.openNextFile();
  }
  dir.rewindDirectory();
  dir.close();
  return bmpcount;
}


bool bmpCheck(File bmpFile) {
   //Check a File to see if it's an appropriate BMP file.
   //Silently fail directories.
   if (bmpFile.isDirectory()){
    return false; 
   }
   #ifdef DEBUG
     Serial.print(F("Checking file '"));
     Serial.print(bmpFile.name());
     Serial.print(F("'..."));
   #endif
   if (read16(bmpFile) == 0x4D42) { // BMP signature
    (void)read32(bmpFile); //Read, but ignore, file size.
    (void)read32(bmpFile); //Same with creator bytes
    (void)read32(bmpFile); //Same with start of image data.
    (void)read32(bmpFile); //Header size.
    (void)read32(bmpFile); //Width.
    (void)read32(bmpFile); //Height.
    (void)read16(bmpFile); //Planes.
    //Now the important stuff!
    //First is Bit Depth and second is Compression.
    if((read16(bmpFile) == 24) && (read32(bmpFile) == 0)){
      //bmpFile.close(); //We don't need the file anymore.
      #ifdef DEBUG
      Serial.println(F("File is a 24-bit BMP"));
      #endif
      return true;
    }
    else {
      //Not a 24-bit BMP file.
      //bmpFile.close();
      #ifdef DEBUG
        Serial.println(F("File is not a 24-bit BMP"));
      #endif
      return false;
    }
  }
  else {
    //bmpFile.close();
    #ifdef DEBUG
      Serial.println(F("File is not a BMP"));
    #endif
    return false;
  }
}

unsigned int randBMP(){
  //Returns a random image number.
  //Makes sure we don't pull the current or previous image, unless there are only two images.
  if (imgnum == 1){
    //Only one image.
    return 1;
  }
  else if (imgnum == 2){
    //Two images. Return the previous image.
    if (imgcur == 1){
      return 2;
    }
    else {
      return 1;
    }
  }
  else {
    unsigned int res = random(1,imgnum+1);
    while ((res == imgcur) or (res == imgprev)){
      res = random(1,imgnum+1);
    }
    return res;
  }
}

void dispBMP(unsigned int bmpnum){
  bmpDraw(grabBMP(bmpnum),0,0);
  if (md == 1){
    drawPlayIcon(146,2);
  }
}

unsigned int numBMP(char* filename){
  #ifdef DEBUG
    Serial.print(F("Finding image "));
    Serial.println(filename);
  #endif
  unsigned int count = 0;
  File dir = SD.open("/");
  dir.rewindDirectory();
  File fi = dir.openNextFile();
  while(fi){
    if (bmpCheck(fi)){
      //A valid BMP.
      count += 1; 
    }
    if (fi.name() == filename){
      fi.close();
      dir.rewindDirectory();
      dir.close();
      return count;
    }
    fi.close();
    delay(5);
    fi = dir.openNextFile();
  }
  dir.rewindDirectory();
  dir.close();
  return 0;
}

char* grabBMP(unsigned int bmpnum){
  if (bmpnum > imgnum) {
    //We don't have that many images.
    //Retrieve the last one.
    bmpnum = imgnum;
  }
  #ifdef DEBUG
    Serial.print(F("Retrieving BMP number "));
    Serial.println(bmpnum);
  #endif
  unsigned int count = 0;
  File dir = SD.open("/");
  dir.rewindDirectory();
  char* bmpName;
  File fi = dir.openNextFile();
  while(fi) {
    if (bmpCheck(fi)){
      //A valid file.
      count += 1; //Increment the count.
      if (count == bmpnum){
        //The file we want
        bmpName = fi.name();
        #ifdef DEBUG
          Serial.print(F("Found "));
          Serial.println(bmpName);
        #endif
        fi.close();
        dir.rewindDirectory();
        dir.close();
        return bmpName;
      }
    }
    //Close the file, move to next one.
    fi.close();
    delay(5);
    fi = dir.openNextFile();
  }
  //Give us the name.
  dir.rewindDirectory();
  dir.close();
  return bmpName;
}
