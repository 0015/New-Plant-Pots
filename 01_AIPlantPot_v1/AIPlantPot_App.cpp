/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#include "AIPlantPot_App.hpp"
static AIPlantPotApp* instance = NULL;

AIPlantPotApp::AIPlantPotApp() {
  instance = this;
  _30sTimeout = false;
  _60sTimeout = false;
}

AIPlantPotApp::~AIPlantPotApp() {}

void systemInfo(System_Event_t event, String param) {
  switch (event) {
    case SYS_TIME:
      if (!instance->_30sTimeout) instance->add_data_to_sdcard(param);
      break;
  }
}
void AIPlantPotApp::init_system() {

  void (*sptr)(System_Event_t, String) = &systemInfo;
  system = new System(systemInfo);

  system->WiFiConnect();
  system->init_SDCard();
  system->read_battery();
}
void AIPlantPotApp::init_epaper() {
  epaper = new EPaper();
}

void AIPlantPotApp::init_sensors() {
  sensors = new Sensors();
}

void AIPlantPotApp::add_data_to_sdcard(String datetime) {
  _30sTimeout = true;

  String sht10data = sensors->get_sht10_data();
  String singleRecord = datetime;
  singleRecord += ",";
  singleRecord += sht10data;
  singleRecord += "\r\n";

  Serial.print("singleRecord:  ");
  Serial.println(singleRecord);

  if (system->append_file(DATA_FILE, singleRecord.c_str())) {
    Serial.println("[OK] APPENDING");
    fetch_data_from_sdcard();

  } else {
    Serial.println("[Fail] APPENDING");
  }
}

void chatTask(void* pvParameters) {
  String answer;
  MyChatGPT* _chatGPT = new MyChatGPT();
  if (_chatGPT->ask(instance->getQuestion(), answer)) {
    Serial.print("OK : ");
    Serial.println(answer);
    instance->epaper->draw_section_3("[*] Tips From ChatGPT", answer.c_str());

  } else {
    Serial.print("FAILED");
    instance->epaper->draw_section_3("[!] Something Wrong", "<No Data from ChatGPT>");
  }
  instance->epaper->update();

  delete (_chatGPT);
  vTaskDelete(NULL);
}


void AIPlantPotApp::fetch_data_from_sdcard() {

  int requireRecords = 14;

  std::vector<std::string> records;
  std::vector<float> temps;
  std::vector<float> humis;



  if (system->read_file(DATA_FILE, records, requireRecords)) {
    Serial.println("[OK] READING");
    for (std::string& record : records) {


      Helper::split(record, ',');

      float temp = std::stof(Helper::strings[1].substr(0, Helper::strings[1].size() - 1));
      temps.push_back(temp);
      float humi = std::stof(Helper::strings[2].substr(0, Helper::strings[2].size() - 1));
      humis.push_back(humi);

      Serial.printf("temp: %.1f, humi: %.1f\n", temp, humi);

      record = Helper::replaceAll(record, std::string(","), std::string("   "));
    }
  }

  epaper->all_clear();
  epaper->draw_section_1(records);
  epaper->draw_section_2(temps, humis);
  epaper->update();

  if (system->isWiFiConnected()) {
    Serial.println("WIFI IS CONNECTED!!!!");
    char question[200];
    sprintf(question, "The soil sensor indicates a temperature %.1fF, humidity %.1f%%. Any tips for the eggplant to grow in?", temps.front(), humis.front());
    Serial.print("Qestion: ");
    Serial.println(question);
    setQuestion(String(question));
    xTaskCreate(chatTask, "chatTask", 10000, NULL, 2, NULL);
  } else {
    Serial.println("WIFI IS NOT CONNECTED!!!!");
    epaper->draw_section_3("[!] Can't connect to WiFi", "<Check your WiFi Credentials>");
  }

  String header = "Last Updated: " + system->get_RTC_time(true);
  String voltage = "--- Voltage: " + String(system->read_battery()) + "V ---";
  epaper->draw_header(header.c_str(), voltage.c_str());
  epaper->update();
}

void AIPlantPotApp::setQuestion(String _question) {
  question = _question;
}

String AIPlantPotApp::getQuestion() {
  return question;
}

void AIPlantPotApp::local_works() {
  add_data_to_sdcard(system->get_RTC_time(false).c_str());
}

void AIPlantPotApp::begin() {
  Serial.println("AIPlantPotApp begin!");

  init_epaper();
  init_sensors();
  init_system();

  //Stop the system if SD Card is not available.
  if (!system->is_SDcard_available()) {
    _30sTimeout = true;
    _60sTimeout = true;
    epaper->all_clear();
    epaper->draw_section_3("[!] Unavailable SD CARD", "<Check SD Card Again>");
    epaper->update();
    epaper->power_off_all();
  }
}

void AIPlantPotApp::loop() {
  if (!_30sTimeout && millis() > TIMEOUT_30SEC) {
    Serial.println("30 second TIMEOUT");
    local_works();
  }

  if (!_60sTimeout && millis() > TIMEOUT_60SEC) {
    _60sTimeout = true;
    Serial.println("60 second TIMEOUT");
    epaper->power_off_all();
    system->deep_sleep(TIME_TO_SLEEP);
  }
}