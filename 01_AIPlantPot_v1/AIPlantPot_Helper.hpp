/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include <vector>

using namespace std;

class Helper {
private:
public:
  inline static string strings[3];
  static int len(string str);
  static void split(string str, char seperator);
  static vector<string> splitWithLength(string& str, int charLen);
  static string replaceAll(string str, const string& from, const string& to);
};