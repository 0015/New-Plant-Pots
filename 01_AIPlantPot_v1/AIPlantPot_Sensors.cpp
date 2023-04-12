/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#include "AIPlantPot_Sensors.hpp"

Sensors::~Sensors() {}

String Sensors::get_sht10_data() {
  float temp_f = sht10.readTemperatureF();
  float humidity = sht10.readHumidity();

  char reading[20];
  sprintf(reading, "%.1fF,%.1f%%", temp_f, humidity);

  return String(reading);
}