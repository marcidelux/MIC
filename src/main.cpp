#include "fs_handler.hpp"
#include "panel_handler.hpp"
#include "cmd_handler.hpp"

#include <NeoPixelBus.h>

#include <Arduino.h>

FsHandler fsh;
PanelHandler *p1;

void test();
void handle_command(Command cmd);

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Starting init...");
  fsh.init();
  Serial.println("Starting testing...");

  p1 = new PanelHandler(2);
}

void loop() {
  test();
  /*
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
  */
}

void test()
{
  for(uint8_t x = 0; x < 16; x++) {
    p1->DrawLine(0, 0, x, 15,RgbPixel{20, 10, 0});
    p1->DrawLine(15, 0, 0, x,RgbPixel{0, 10, 20});
    p1->Show();
    delay(100);
    p1->DrawLine(0,0,x,15,RgbPixel{0, 0, 0});
    p1->DrawLine(15, 0, 0, x,RgbPixel{0, 00, 0});
  }

  for(int8_t y = 15; y>= 0; y--) {
    p1->DrawLine(0,0,15,y,RgbPixel{20, 10, 0});
    p1->DrawLine(15, 0, 15-y, 15,RgbPixel{0, 10, 20});
    p1->Show();
    delay(100);
    p1->DrawLine(0,0,15,y,RgbPixel{0, 0, 0});
    p1->DrawLine(15, 0, 15-y, 15, RgbPixel{0, 0, 0});
  }
  /*
  uint16_t stripIdx = 0;
  for(uint8_t row = 0; row < PANEL_ROWS; row++) {
    for (uint8_t col = 0; col < PANEL_COLS; col++) {
      RgbPixel pixcol{0, 10, 0};
      //Serial.printf("row: %d\tcol: %d\tR: %d\tG: %d\tB :%d\n", row, col, pixcol.R, pixcol.G, pixcol.B);
      p1->SetPixelColor(row, col, pixcol);
      //p1->_strip.SetPixelColor(stripIdx, RgbColor(0,0,10));
      p1->Show();
      delay(10);
      p1->SetPixelColor(row, col, RgbPixel{0,0,0});
      //p1->_strip.SetPixelColor(stripIdx, RgbColor(0,0,0));
      stripIdx++;
    }
  }
  */

  Serial.println("finished");

  /*
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
  */
}

void handle_command(Command cmd) {
  /*
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
  */
}