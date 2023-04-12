/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#ifndef __AIPLANTPOT_CONFIG_H
#define __AIPLANTPOT_CONFIG_H

#include <Arduino.h>

#define WIFI_SSID "<WIFI_SSID>"
#define WIFI_PW "<WIFI_PW>"

#define OPENAI_API_KEY "<OpenAI_API_KEY>"  // https://platform.openai.com/account/api-keys

#define TIMEOUT_30SEC 30 * 1000  // 30 Seconds
#define TIMEOUT_60SEC 60 * 1000  // 60 Seconds

#define TIME_ZONE "PST8PDT,M3.2.0,M11.1.0"  // https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h

#define DATA_FILE "/datafile.csv"

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP 60           // 1 MIN, Time ESP32 will go to sleep (in seconds)

#endif
