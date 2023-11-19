#include "fs_handler.hpp"

FsHandler::FsHandler()
{
}

FsHandler::~FsHandler()
{
}

uint8_t FsHandler::init()
{
   if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
      return 1;
   }

   return 0;
}

/*
Pixel *FsHandler::readImg16x16(const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = SPIFFS.open(path);
    if(!file || file.isDirectory()){
        Serial.println("− failed to open file for reading");
        return nullptr;
    }

    Serial.println("− read from file:");
    uint16_t i = 0;
    Pixel* img = new Pixel[256];
    int data = 0;
    while(file.available()){
        data = file.read();
        if (data != -1) {
            img[i].R = uint8_t(data);
        } 
        data = file.read();
        if (data != -1) {
            img[i].G = uint8_t(data);
        } 
        data = file.read();
        if (data != -1) {
            img[i].B = uint8_t(data);
        }
        i++; 
    }

    return img;
}

uint8_t FsHandler::writeImg16X16(const char * path, const Pixel *img)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = SPIFFS.open(path, FILE_WRITE);
    if(!file){
        Serial.println("− failed to open file for writing");
        return 1;
    }

    for (uint16_t i = 0; i < 256; i++) {
        file.write(img[i].R);
        file.write(img[i].G);
        file.write(img[i].B);
    }

    return 0;
}
*/