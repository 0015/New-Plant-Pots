/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#pragma once
#include "AIPlantPot_System.hpp"
#include "AIPlantPot_EPaper.hpp"
#include "AIPlantPot_Sensors.hpp"
#include "AIPlantPot_Helper.hpp"
#include "AIPlantPot_ChatGPT.hpp"
#include "pins.h"

class AIPlantPotApp {
private:

  String question;

  bool _60sTimeout;

  void init_system();
  void init_epaper();
  void init_sensors();
  void fetch_data_from_sdcard();
  void local_works();

public:
  bool _30sTimeout;
  EPaper *epaper;
  System *system;
  Sensors *sensors;

  AIPlantPotApp();
  ~AIPlantPotApp();
  void add_data_to_sdcard(String datetime);
  void setQuestion(String question);
  String getQuestion();
  void begin();
  void loop();
};
