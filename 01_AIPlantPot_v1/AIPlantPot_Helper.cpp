/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#include "AIPlantPot_Helper.hpp"

int Helper::len(string str) {
  int length = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    length++;
  }
  return length;
}

void Helper::split(string str, char seperator) {
  int currIndex = 0, i = 0;
  int startIndex = 0, endIndex = 0;
  while (i <= len(str)) {
    if (str[i] == seperator || i == len(str)) {
      endIndex = i;
      string subStr = "";
      subStr.append(str, startIndex, endIndex - startIndex);
      strings[currIndex] = subStr;
      currIndex += 1;
      startIndex = endIndex + 1;
    }
    i++;
  }
}

vector<string> Helper::splitWithLength(string& str, int charLen) {
  vector<string> substrings;
  for (size_t i = 0; i < str.size(); i += charLen) {
    substrings.push_back(str.substr(i, charLen));
  }
  return substrings;
}


string Helper::replaceAll(string str, const string& from, const string& to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}