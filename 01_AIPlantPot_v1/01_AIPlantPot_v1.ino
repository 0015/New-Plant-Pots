/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////

/*
- Device
T5 4.7 Inch E-paper V2.3 (ESP32-S3)
https://www.lilygo.cc/products/t5-4-7-inch-e-paper-v2-3

- Required Library
Arduino ESP32: 2.0.7

LilyGo EPD47: 0.3.0
https://github.com/Xinyuan-LilyGO/LilyGo-EPD47/tree/esp32s3
// This is not based on the latest version of EPDiy E-Paper Driver.
// Features such as screen rotation are not yet available.

ChatGPT Client: 0.1.2
https://github.com/0015/ChatGPT_Client_For_Arduino

SHT1x Temperature / Humidity Sensor Library for Arduino
https://github.com/practicalarduino/SHT1x
*/

//Please update the necessary information in the AIPlantPot_Config.h file.

#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#include "AIPlantPot_App.hpp"

AIPlantPotApp *_AIPlantPot_App;

void setup() {
  Serial.begin(115200);

  _AIPlantPot_App = new AIPlantPotApp();
  _AIPlantPot_App->begin();
}

void loop() {
  _AIPlantPot_App->loop();
}