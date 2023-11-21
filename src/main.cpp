#include "fs_handler.hpp"
#include "panel_handler.hpp"
#include "cmd_handler.hpp"
#include "msgeq7_handler.hpp"

#include <NeoPixelBus.h>

#include <Arduino.h>


#define MSGEQ7_ANALOG_PIN 15
#define MSGEQ7_STROBE_PIN 4
#define MSGEQ7_RESET_PIN 16 


FsHandler fsh;
PanelHandler *p1;
Msgeq7 *meh;

void test();
void handle_command(Command cmd);

void setup() {
  Serial.begin(9600);
  delay(3000);
  Serial.println("Starting init...");
  fsh.init();
  Serial.println("Starting testing...");
  randomSeed(analogRead(0));
  p1 = new PanelHandler(2);
  meh = new Msgeq7(MSGEQ7_RESET_PIN, MSGEQ7_STROBE_PIN, MSGEQ7_ANALOG_PIN);
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
 //test();
  //delay(500);
  //meh->test();
}

void test()
{
  RgbPixel red{50,0,0};
  RgbPixel green{0,50,0};
  RgbPixel blue{0,0,50};
  RgbPixel black{0,0,0};
  
  RgbPixel rgb[3] = {red, green, blue};
  String colors[3] = {"red", "green", "blue"};

  for (uint8_t i = 0; i < 3; i++) {
    Serial.println(colors[i]);
    p1->DrawCircle(8,8,5,rgb[i]);
    p1->Show();
    delay(2000);
  }

  /*
  for(uint8_t x = 0; x < 16; x++) {
    p1->DrawLine(0, 0, x, 15,RgbPixel{20, 10, 0});
    p1->DrawLine(15, 0, 0, x,RgbPixel{0, 10, 20});
    p1->DrawLine(5, 5 + x/3, 10, 5 + x/3,RgbPixel{0, 10, 0});
    p1->Show();
    delay(100);
    p1->DrawLine(0,0,x,15,RgbPixel{0, 0, 0});
    p1->DrawLine(15, 0, 0, x,RgbPixel{0, 00, 0});
    p1->DrawLine(5, 5 + x/3, 10, 5 + x/3,RgbPixel{0, 0, 0});
  }

  for(int8_t y = 15; y>= 0; y--) {
    p1->DrawLine(0,0,15,y,RgbPixel{20, 10, 0});
    p1->DrawLine(15, 0, 15-y, 15,RgbPixel{0, 10, 20});
    p1->DrawLine(5, 5 - y/3, 10, 5 - y/3,RgbPixel{0, 10, 0});
    p1->Show();
    delay(100);
    p1->DrawLine(0,0,15,y,RgbPixel{0, 0, 0});
    p1->DrawLine(15, 0, 15-y, 15, RgbPixel{0, 0, 0});
    p1->DrawLine(5, 5 - y/3, 10, 5 - y/3,RgbPixel{0, 10, 0});
  }
  */
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
  switch (cmd.commandType)
  {
    case CMD_WRIE_IMAGE:
      {
        Serial.printf("writing image: %s\n", cmd.parameter);
        switch (fsh.WriteData((char*)cmd.parameter, cmd.data))
        {
          case 0:
            Serial.println("image saved.");
            break;
          case 1:
            Serial.println("failed to open file for writing");
            break;
          case 2:
            Serial.println("failedd to write file");
            break;
        }
        break;
      }
    case CMD_SHOW_IMAGE:
      {
        Serial.printf("showing image: %s\n", cmd.parameter);
        fsh.ReadData((char*)cmd.parameter);
        p1->SetStripBuffer(fsh.GetBuffer16x16());
        p1->Show();
      }
      break;
    case CMD_DELETE_IMAGE:
    {
      switch(fsh.RemoveImage((char*)cmd.parameter))
      {
        case false:
          Serial.println("Failed to remove image");
          break;
        case true:
          Serial.println("Removed image succesfully");
          break;
      }
      break;
    }
    case CMD_READ_DIR:
    {
      Serial.printf("read dir:");
      String dirs = fsh.ReadDirectory("/");
      Serial.println(dirs);
    }
  }
}