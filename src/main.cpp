#include "fs_handler.hpp"
#include "img_handler.hpp"
#include "cmd_handler.hpp"

#include <NeoPixelBus.h>

#include <Arduino.h>

const uint16_t PixelCount = 256; // make sure to set this to the number of your LEDs.
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, digital out, GPIO
NeoPixelBus<NeoGrbFeature, NeoEsp32I2s1X8Ws2812xMethod> strip(PixelCount, PixelPin);
const RgbColor CylonEyeColor(HtmlColor(0x7f0000));

FsHandler fsh;
ImgHandler imgh;

void test();
void handle_command(Command cmd);

void setup() {
  Serial.begin(9600);
  delay(4000);
  Serial.println("Starting init...");
  fsh.init();
  Serial.println("Starting testing...");
  strip.Begin();
  //test();
}

void loop() {
  // Call readCommand and store the result in a tuple
  std::tuple<Command, uint8_t> result = readCommand();

  // Extract the command and status from the tuple
  Command cmd = std::get<0>(result);
  uint8_t status = std::get<1>(result);

  // Check the status and process the command
  if (status == 0) {
    Serial.println("Received data");
    Serial.printf("type: %d, paramlen: %d, datalen: %d\n", cmd.commandType, cmd.parameterLength, cmd.dataLength);
    
    handle_command(cmd);

    clearCmd(&cmd);
  }
}

void test()
{
    Pixel* img_write = new Pixel[256]; 

    for (uint16_t i = 0; i < 256; i++) {
        img_write[i].R = i;
        img_write[i].G = i+1;
        img_write[i].B = i+2;
    }

    if (fsh.writeImg16X16("/test.img16", img_write)) {
        Serial.println("Error while writing IMG");
    }

    Pixel* img_read = fsh.readImg16x16("/test.img16"); 

    Serial.println("TEST IMAGE");
    for(uint16_t i = 0; i < 256; i++) {
        Serial.printf("%d:\tR:%d\tG:%d\tB:%d\t\n", i, img_read[i].R, img_read[i].G, img_read[i].B);
        delay(10);
    }

    imgh.showImg16(img_read);
}

void handle_command(Command cmd) {
  switch (cmd.commandType)
  {
  case CMD_WRIE_IMAGE:
    {
      Serial.printf("writing image: %s\n", cmd.parameter);
      for (uint16_t i = 0; i < cmd.dataLength; i++) {
        Serial.printf("i:%d\tb:%d", i, cmd.data[i]);
      }
      
      Pixel *img_write = imgh.create16fromBytes(cmd.data);
      fsh.writeImg16X16((char*)cmd.parameter, img_write);
      delete[] img_write;
      break;
    }
  case CMD_SHOW_IMAGE:
    {
      Serial.printf("showing image: %s\n", cmd.parameter);
      Pixel* img_read = fsh.readImg16x16((char*)cmd.parameter); 
      
      for (uint16_t y = 0; y < 16; y++) {
        for(uint16_t x = 0; x < 16; x++) {
          uint16_t i = y * 16 + x;
          Serial.printf("X: %d Y: %d, I: %d - R: %d, G: %d B: %d\n", x, y, i, img_read[i].R, img_read[i].G, img_read[i].B);
        }
      }
      
      imgh.showImg16(img_read);
      delete[] img_read;
      break;
    }
  }
}