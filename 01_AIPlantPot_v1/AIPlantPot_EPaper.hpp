/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#pragma once
#include <Arduino.h>
#include <vector>
#include <string>
#include "epd_driver.h"
#include "AIPlantPot_Helper.hpp"
#include "font/opensans8.h"
#include "font/opensans10b.h"
#include "font/firasans_12.h"
enum alignment { LEFT,
                 RIGHT,
                 CENTER };
#define White 0xFF
#define LightGrey 0xBB
#define Grey 0x88
#define DarkGrey 0x44
#define Black 0x00

class EPaper {
private:
  uint8_t *framebuffer;
  GFXfont currentFont;
  int cursor_x = 20;
  int cursor_y = 30;
  int section_width = EPD_WIDTH / 3 - 20;
  int section_height = EPD_HEIGHT - 70;
  void setFont(GFXfont const &font);
  void drawString(int x, int y, String text, alignment align);
  void fillCircle(int x, int y, int r, uint8_t color);
  void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color);
  void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void drawCircle(int x0, int y0, int r, uint8_t color);
  void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  void drawPixel(int x, int y, uint8_t color);

public:
  EPaper();
  ~EPaper();
  void draw_section_1(std::vector<std::string> records);
  void draw_section_2(std::vector<float> temps, std::vector<float> humids);
  void draw_section_3(const char *title, std::string contents);
  void draw_header(std::string lastupdated, std::string voltage);
  void draw_graph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode);
  void power_off_all();
  void all_clear();
  void update();
};