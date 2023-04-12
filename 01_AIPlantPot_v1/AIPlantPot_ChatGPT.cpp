/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#include "AIPlantPot_ChatGPT.hpp"

MyChatGPT::~MyChatGPT() {}

bool MyChatGPT::ask(String question, String& answer) {

  if (chat_gpt.simple_message("gpt-3.5-turbo-0301", "user", question, answer)) {
    return true;
  } else {
    return false;
  }
}