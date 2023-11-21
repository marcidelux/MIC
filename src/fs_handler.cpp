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

uint8_t FsHandler::WriteData(const char *path, uint8_t *buffer)
{
    File file = SPIFFS.open(path, FILE_WRITE);
    if(!file){
        return 1;
    }

    size_t written = file.write(buffer, BUFF_SIZE_16X16);

    if (written != BUFF_SIZE_16X16) {
        return 2;
    }

    return 0;
}

uint8_t FsHandler::ReadData(const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = SPIFFS.open(path);
    if(!file || file.isDirectory()){
        Serial.println("− failed to open file for reading");
        return 1;
    }

    if (file.read(_buffer_16x16, BUFF_SIZE_16X16) != BUFF_SIZE_16X16) {
        return 2;
    }

    return 0;
}

String FsHandler::ReadDirectory(const char *dir)
{
    String directoryContents;
    directoryContents += "Listing directory: " + String(dir) + "\n";

    File root = SPIFFS.open(dir);
    if (!root) {
        directoryContents += "Failed to open directory\n";
        return directoryContents;
    }
    if (!root.isDirectory()) {
        directoryContents += "Not a directory\n";
        return directoryContents;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            directoryContents += "  DIR : " + String(file.name()) + "\n";
            ReadDirectory(file.name());
        } else {
            directoryContents += "  FILE: " + String(file.name()) + "\tSIZE: " + String(file.size()) + "\n";
        }
        file = root.openNextFile();
    }
    return directoryContents;
}

uint8_t FsHandler::RemoveImage(const char *path)
{
    return SPIFFS.remove(path);
}

uint8_t *FsHandler::GetBuffer16x16()
{
    return _buffer_16x16;
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