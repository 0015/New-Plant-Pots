/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#pragma once
#include "AIPlantPot_Config.h"
#include <WiFiClientSecure.h>
#include <vector>
#include "esp_adc_cal.h"
#include "esp32-hal.h"
#include "lwip/apps/sntp.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "pins.h"
#if defined(T5_47_PLUS)
#include <Wire.h>
#include "pcf8563.h"
#endif

typedef enum {
  SYS_TIME,
  SYS_BATTERY,
} System_Event_t;

class System {
private:
#if defined(T5_47_PLUS)
  PCF8563_Class *rtc;
#endif

  bool isSDCardAvailable;
  typedef void (*FuncPtrString)(System_Event_t, String);

  int vRef = 1100;
  void initADC();
public:
  fs::FS &storage = SD;
  FuncPtrString system_event_cb;

  System(FuncPtrString callback);
  ~System();

  void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);
  void WiFiConnect();
  bool isWiFiConnected();

  void set_config_tz_time();
  void update_local_time();
  String get_local_time_for_msg();
  String get_RTC_time(bool isFull);

  void init_SDCard();
  bool is_SDcard_available();

  bool append_file(const char *path, const char *message);
  int read_file_size(const char *path);
  bool read_file(const char *path, std::vector<std::string> &records, int reqRecords);
  void sdcard_info(unsigned long *totalBytes, unsigned long *usedBytes);

  float read_battery();
  void update();
  void restart();
  void deep_sleep(int seconds);
};