/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#pragma once
#include <SHT1x-ESP.h>
#include "pins.h"

class Sensors {
private:
  SHT1x sht10;

public:
  Sensors()
    : sht10(GPIO_MISO, GPIO_SCLK, SHT1x::Voltage::DC_3_3v) {}
  ~Sensors();
  String get_sht10_data();
};