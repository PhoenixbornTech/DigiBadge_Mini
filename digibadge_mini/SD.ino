void startSD(){
  //We haven't checked any buttons yet, so do it manually.
  if (!digitalRead(SDCD)) {
    //We have an SD card physically present. Attempt to start it.
    bitSet(bobs, 3); //To prevent from immediately resetting.
    if (SD.begin(SDCS)) {
      imgnum = countBMP();
      if (imgnum > 0) {
        //"Current Image" starts at 1. Adjust accordingly.
        imgcur = 1;
      }
    }
  }
}

uint16_t countBMP(){
  uint16_t bmpcount = 0;
  //Open directory, then rewind it. Just in case someone hasn't been kind.
  File dir = SD.open("/");
  dir.rewindDirectory();
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
   //if (bmpFile.isDirectory()){
   // return false; 
   //}
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
      return true;
    }
    //else {
      //Not a 24-bit BMP file.
    //  return false;
    //}
  }
  else {
    //File is not a BMP
    return false;
  }
}

uint16_t randBMP(){
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
    uint16_t res = random(1,imgnum+1);
    while (res == imgcur){
      res = random(1,imgnum+1);
    }
    return res;
  }
}

uint16_t numBMP(char* filename){
  uint16_t count = 0;
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

char* grabBMP(uint16_t bmpnum){
  if (bmpnum > imgnum) {
    //We don't have that many images.
    //Retrieve the last one.
    bmpnum = imgnum;
  }
  uint16_t count = 0;
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
