/////////////////////////////////////////////////////////////////
/*
  ESP32-S3 Smart Plant Pot with Real-Time Soil Data Analysis on E-Paper Display
  For More Information: https://youtu.be/x3e_vu3tHkg
  Created by Eric N. (ThatProject)
*/
/////////////////////////////////////////////////////////////////
#include "AIPlantPot_EPaper.hpp"

EPaper::EPaper() {
  epd_init();

  framebuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!framebuffer) {
    Serial.println("alloc memory failed !!!");
    while (1)
      ;
  }
  memset(framebuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

EPaper::~EPaper() {}

void EPaper::draw_section_1(std::vector<std::string> records) {

  epd_draw_rect(10, 40, section_width, section_height, 0, framebuffer);

  cursor_x = 24;
  cursor_y = 74;
  write_string((GFXfont *)&OpenSans10B, " Date&Time    Temp   Humi", &cursor_x, &cursor_y, framebuffer);

  int data_cursor_x = 18;
  int data_cursor_y = 112;

  int recordIdx = 0;
  for (int yPos = 90; yPos < 510;) {
    epd_draw_hline(10, yPos, section_width, 0, framebuffer);
    yPos += 30;
    if (recordIdx < records.size()) {
      writeln((GFXfont *)&FiraSans_12, records[recordIdx].c_str(), &data_cursor_x, &data_cursor_y, framebuffer);
      ++recordIdx;
    }

    data_cursor_x = 18;
    data_cursor_y += 30;
  }

  epd_draw_vline(156, 40, section_height, 0, framebuffer);
  epd_draw_vline(230, 40, section_height, 0, framebuffer);
}

void EPaper::draw_section_2(std::vector<float> temps, std::vector<float> humids) {

  epd_draw_rect(330, 40, section_width, section_height, 0, framebuffer);

  int gwidth = 240, gheight = 150;
  int gx = 376;
  int gy = 84;

  float *temperature_readings = &temps[0];
  float *humidity_readings = &humids[0];

  draw_graph(gx, gy, gwidth, gheight, 0, 150, "Temperature", temperature_readings, temps.size(), true, false);
  draw_graph(gx, gy + gheight + 70, gwidth, gheight, 0, 100, "Humidity", humidity_readings, humids.size(), false, false);
}

void EPaper::draw_section_3(const char *title, std::string contents) {

  epd_poweron();
  Rect_t area = {
    .x = 650,
    .y = 40,
    .width = section_width,
    .height = section_height,
  };

  epd_clear_area(area);

  epd_draw_rect(650, 40, section_width, section_height, 0, framebuffer);
  cursor_x = 674;
  cursor_y = 74;
  write_string((GFXfont *)&OpenSans10B, title, &cursor_x, &cursor_y, framebuffer);

  if (contents.length() > 600) {
    contents.resize(600);  //MAX
    contents.append("...");
  }

  cursor_x = 674;
  cursor_y = 100;
  int n = 32;
  std::vector<std::string> tokens = Helper::splitWithLength(contents, n);
  for (auto &token : tokens) {
    writeln((GFXfont *)&OpenSans8, token.c_str(), &cursor_x, &cursor_y, framebuffer);
    cursor_x = 674;
    cursor_y += 20;
  }
}

void EPaper::draw_header(std::string lastupdated, std::string voltage) {
  Rect_t area = {
    .x = 0,
    .y = 0,
    .width = section_width,
    .height = 40,
  };

  epd_clear_area(area);
  cursor_x = 10;
  cursor_y = 30;
  write_string((GFXfont *)&FiraSans_12, lastupdated.c_str(), &cursor_x, &cursor_y, framebuffer);

  cursor_x = EPD_WIDTH - 260;
  cursor_y = 30;
  write_string((GFXfont *)&FiraSans_12, voltage.c_str(), &cursor_x, &cursor_y, framebuffer);
}

void EPaper::setFont(GFXfont const &font) {
  currentFont = font;
}

void EPaper::drawString(int x, int y, String text, alignment align) {
  char *data = const_cast<char *>(text.c_str());
  int x1, y1;  //the bounds of x,y and w and h of the variable 'text' in pixels.
  int w, h;
  int xx = x, yy = y;
  get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
  if (align == RIGHT) x = x - w;
  if (align == CENTER) x = x - w / 2;
  int cursor_y = y + h;
  write_string(&currentFont, data, &x, &cursor_y, framebuffer);
}

void EPaper::fillCircle(int x, int y, int r, uint8_t color) {
  epd_fill_circle(x, y, r, color, framebuffer);
}

void EPaper::drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color) {
  epd_draw_hline(x0, y0, length, color, framebuffer);
}

void EPaper::drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color) {
  epd_draw_vline(x0, y0, length, color, framebuffer);
}

void EPaper::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  epd_write_line(x0, y0, x1, y1, color, framebuffer);
}

void EPaper::drawCircle(int x0, int y0, int r, uint8_t color) {
  epd_draw_circle(x0, y0, r, color, framebuffer);
}

void EPaper::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  epd_draw_rect(x, y, w, h, color, framebuffer);
}

void EPaper::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  epd_fill_rect(x, y, w, h, color, framebuffer);
}

void EPaper::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                          int16_t x2, int16_t y2, uint16_t color) {
  epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, framebuffer);
}

void EPaper::drawPixel(int x, int y, uint8_t color) {
  epd_draw_pixel(x, y, color, framebuffer);
}

void EPaper::draw_graph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode) {
#define auto_scale_margin 0  // Sets the autoscale increment, so axis steps up fter a change of e.g. 3
#define y_minor_axis 5       // 5 y-axis division markers
  setFont(OpenSans10B);
  int maxYscale = -10000;
  int minYscale = 10000;
  int last_x, last_y;
  float x2, y2;
  if (auto_scale == true) {
    for (int i = 1; i < readings; i++) {
      if (DataArray[i] >= maxYscale) maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale) minYscale = DataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin);  // Auto scale the graph and round to the nearest value defined, default was Y1Max
    Y1Max = round(maxYscale + 0.5);
    if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin);  // Auto scale the graph and round to the nearest value defined, default was Y1Min
    Y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
  drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, Grey);
  drawString(x_pos - 20 + gwidth / 2, y_pos - 28, title, CENTER);
  for (int gx = 0; gx < readings; gx++) {
    x2 = x_pos + gx * gwidth / (readings - 1) - 1;  // max_readings is the global variable that sets the maximum data that can be plotted
    y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
    if (barchart_mode) {
      fillRect(last_x + 2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 2, Black);
    } else {
      drawLine(last_x, last_y - 1, x2, y2 - 1, Black);  // Two lines for hi-res display
      drawLine(last_x, last_y, x2, y2, Black);
    }
    last_x = x2;
    last_y = y2;
  }
  //Draw the Y-axis scale
#define number_of_dashes 20
  for (int spacing = 0; spacing <= y_minor_axis; spacing++) {
    for (int j = 0; j < number_of_dashes; j++) {  // Draw dashed graph grid lines
      if (spacing < y_minor_axis) drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), Grey);
    }
    if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 5 || title == "TXT_PRESSURE_IN") {
      drawString(x_pos - 10, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
    } else {
      if (Y1Min < 1 && Y1Max < 10) {
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
      } else {
        drawString(x_pos - 7, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0), RIGHT);
      }
    }
  }
  for (int i = 0; i < 3; i++) {
    drawString(20 + x_pos + gwidth / 3 * i, y_pos + gheight + 10, String(i * 15) + "m", LEFT);
    if (i < 2) drawFastVLine(x_pos + gwidth / 3 * i + gwidth / 3, y_pos, gheight, LightGrey);
  }
}

void EPaper::power_off_all() {
  epd_poweroff_all();
}

void EPaper::all_clear() {
  epd_poweron();
  epd_clear();
  epd_poweroff();
}

void EPaper::update() {
  epd_draw_grayscale_image(epd_full_screen(), framebuffer);
  epd_poweroff();
}