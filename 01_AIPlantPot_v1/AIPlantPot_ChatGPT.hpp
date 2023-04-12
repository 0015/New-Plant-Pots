/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#pragma once
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <ChatGPT.hpp>
#include "AIPlantPot_Config.h"

class MyChatGPT {
private:
  WiFiClientSecure client;
  ChatGPT<WiFiClientSecure> chat_gpt;

public:
  MyChatGPT()
    : chat_gpt(&client, "v1", OPENAI_API_KEY) {
    client.setInsecure();
  }
  ~MyChatGPT();
  bool ask(String question, String& answer);
};