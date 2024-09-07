#include "fs_handler.hpp"
#include "controller.hpp"
#include <Arduino.h>

FsHandler fsh;
Controller controller;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting init...");
  controller.Init();
  Serial.println("Start loop...");
  //fsh.init();
  //randomSeed(analogRead(0));
}

void loop() {
  controller.CalcAndShow();
}