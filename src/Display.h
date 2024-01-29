#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <FreeDefaultFonts.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>

#include "CanBusReceiver.h"

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define BLACK 0x0000
#define WHITE 0xFFFF
#define CYAN 0x07FF
#define YELLOW 0xFFE0
#define GREEN 0x07E0
#define GRAY 0x8410

#define SHOW_COORDINATES_ON false // change this to view touch point coordinates in serial monitor

class Display
{
private:
  MCUFRIEND_kbv &tft;
  TouchScreen &ts;
  Adafruit_GFX_Button *page_0_btn[3];
  Adafruit_GFX_Button *page_1_btn[3];
  Adafruit_GFX_Button *page_2_btn[7];

  Adafruit_GFX_Button
      plant_light_on_btn,
      nextBtn,
      feed_btn,
      start_btn,
      temp_btn,
      level_btn,
      backBtn,
      pumpBtn,
      led_btn_on,
      timer_btn_on;

  const int XP = 6, XM = A2, YP = A1, YM = 7;
  const int TS_LEFT = 973, TS_RT = 187, TS_TOP = 247, TS_BOT = 803; // changed to landscape

  uint8_t newPercent, lastPercent, currentpage, prevPage = 0;
  double coolantTempDisplayed = 0.0;

  int pixel_x, pixel_y;
  bool feed_timer_on = false;
  bool water_pump_state = false;
  bool led_relay_state = false;
  bool plant_light_state = true;
  bool timer_on = true;
  bool initalPageRender = false;

  char next[6] = "NEXT ";
  char back[6] = "BACK ";
  char text_1[7] = "text 1";
  char text_2[7] = "text 2";
  char on[4] = "ON";
  char off[5] = "OFF";

  void drawScale_1();
  void drawScale_2();
  void drawScale_3();
  void drawScale_4();
  bool TouchGetXY(void);
  void drawOldBar(int oldPer);
  void draw_button_list(Adafruit_GFX_Button **pb);
  bool update_button(Adafruit_GFX_Button *b, bool down);
  void draw_button_state(int x, int y, int width, int height, int sz, int text_color, int box_color, char *msg);

public:
  Display(MCUFRIEND_kbv &tft, TouchScreen &ts);
  void page_0(void);
  void page_1(CanBusReceiver &data);
  void page_2(void);
  void draw_water_temp(CanBusReceiver &data);
  void showmsgXY(int x, int y, int sz, const GFXfont *f, int color, const char *msg);
  void printInteger(int x, int y, int sz, const GFXfont *f, int color, const int number);
  void printDouble(int x, int y, int sz, const GFXfont *f, int color, const double number);
  bool update_button_list();
  void draw_output_state(int index);
  Adafruit_GFX_Button getNextBtn();
  Adafruit_GFX_Button getBackBtn();
  Adafruit_GFX_Button getPumpBtn();
  uint8_t getCurrentPage();
};

#endif