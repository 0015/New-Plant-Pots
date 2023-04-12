/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#include "AIPlantPot_System.hpp"

static System* instance = NULL;

System::System(FuncPtrString callback) {
  instance = this;
  isSDCardAvailable = false;
  system_event_cb = callback;

#if defined(T5_47_PLUS)
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  rtc = new PCF8563_Class();
  rtc->begin();
#endif
  initADC();
}
System::~System() {}

extern "C" void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  instance->WiFiEvent(event, info);
}

extern "C" void WiFiEventOn() {
  WiFi.onEvent(WiFiEvent);
}

void System::WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  switch (event) {
    case SYSTEM_EVENT_STA_DISCONNECTED:
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      set_config_tz_time();
      break;

    default:
      break;
  }
}

void System::WiFiConnect() {
  WiFi.disconnect(true);
  WiFiEventOn();
  WiFi.begin(WIFI_SSID, WIFI_PW);
}

bool System::isWiFiConnected() {
  return WiFi.isConnected();
}

void System::set_config_tz_time() {
  const char* ntpServer0 = "0.north-america.pool.ntp.org";
  const char* ntpServer1 = "0.north-america.pool.ntp.org";
  const char* ntpServer2 = "time1.google.com";
  esp_netif_init();
  if (sntp_enabled()) {
    sntp_stop();
  }
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, (char*)ntpServer0);
  sntp_setservername(1, (char*)ntpServer1);
  sntp_setservername(2, (char*)ntpServer2);
  sntp_init();
  setenv("TZ", TIME_ZONE, 1);
  tzset();

  update_local_time();
}

void System::update_local_time() {

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }
#if defined(T5_47_PLUS)
  rtc->syncToRtc();
#endif

  char miniTimestamp[12];
  strftime(miniTimestamp, 12, "%m-%d %H:%M", &timeinfo);

  system_event_cb(SYS_TIME, miniTimestamp);
}

String System::get_local_time_for_msg() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "";
  }

  char dateTime[10];
  strftime(dateTime, 10, "%T", &timeinfo);
  return String(dateTime);
}

String System::get_RTC_time(bool isFull) {

  RTC_Date t = rtc->getDateTime();
  char format[128];
  if (isFull)
    snprintf(format, sizeof(format), "%d-%02d-%02d %02d:%02d:%02d", t.year, t.month, t.day, t.hour, t.minute, t.second);
  else
    snprintf(format, sizeof(format), "%02d-%02d %02d:%02d", t.month, t.day, t.hour, t.minute);

  return String(format);
}

void System::init_SDCard() {

  SPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  isSDCardAvailable = SD.begin(SD_CS, SPI);
}

bool System::append_file(const char* path, const char* message) {
  File file = storage.open(path, FILE_APPEND);
  if (!file) {
    file.close();
    return false;
  }
  if (!file.print(message)) {
    file.close();
    return false;
  }
  file.close();
  return true;
}

int System::read_file_size(const char* path) {
  File file = storage.open(path, FILE_READ);
  if (!file) {

    file.close();
    return 0;
  }

  return file.size();
}

bool System::read_file(const char* path, std::vector<std::string>& records, int reqRecords) {
  File file = storage.open(path, FILE_READ);
  if (!file) {
    file.close();
    return false;
  }

  int numRecords = 0;
  int totalRecords = 0;
  while (file.available()) {
    file.readStringUntil('\n');
    ++totalRecords;
  }

  if (totalRecords <= reqRecords) {
    totalRecords = 0;
  } else {
    totalRecords -= reqRecords;
  }

  file.seek(0);
  while (file.available()) {
    String field = file.readStringUntil('\n');
    if (numRecords >= totalRecords) {
      records.insert(records.begin(), field.c_str());
    }
    numRecords++;
  }

  file.close();
  return true;
}

void System::sdcard_info(unsigned long* totalBytes, unsigned long* usedBytes) {
  *totalBytes = SD.totalBytes() / (1024 * 1024);
  *usedBytes = SD.usedBytes() / (1024 * 1024);
}

bool System::is_SDcard_available() {
  return isSDCardAvailable;
}

// Battery
void System::initADC() {
  // Correct the ADC reference voltage
  esp_adc_cal_characteristics_t adc_chars;
#if defined(T5_47)
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
    ADC_UNIT_1,
    ADC_ATTEN_DB_11,
    ADC_WIDTH_BIT_12,
    1100,
    &adc_chars);
#else
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(
    ADC_UNIT_2,
    ADC_ATTEN_DB_11,
    ADC_WIDTH_BIT_12,
    1100,
    &adc_chars);
#endif
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    vRef = adc_chars.vref;
  }
}

float System::read_battery() {

  uint16_t v = analogRead(BATT_PIN);
  return ((float)v / 4095.0) * 2.0 * 3.3 * (vRef / 1000.0);
}

void System::update() {
  this->update_local_time();
  this->read_battery();
}

void System::restart() {
  ESP.restart();
}

void System::deep_sleep(int seconds) {
  esp_sleep_enable_timer_wakeup(seconds * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}