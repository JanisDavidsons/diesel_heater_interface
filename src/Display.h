#ifndef DISPLAY_H
#define DISPLAY_H

#include <SD.h>
#include <Arduino.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <FreeDefaultFonts.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>



#include "CanBusReceiver.h"

#define SD_CS 10     // SD card pin on your shield
#define BUFFPIXEL 20 // Drawing speed, 20 is meant to be the best but you can use 60 altough it takes a lot of uno's RAM

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define RED 0xF800
#define CYAN 0x07FF
#define GRAY 0x8410
#define WHITE 0xFFFF
#define GREEN 0x07E0
#define BLACK 0x0000
#define SILVER 0xC618
#define YELLOW 0xFFE0
#define TEXT_BACKGROUND 0x4A69

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
      buttonOne,
      buttonTwo,
      buttonThree,
      buttonFour,
      buttonGrid,
      buttonSix,
      buttonSeven,
      buttonEight,
      buttonNext,
      buttonBack;

  const int XP = 6, XM = A2, YP = A1, YM = 7;
  const int TS_LEFT = 973, TS_RT = 187, TS_TOP = 247, TS_BOT = 803; // changed to landscape

  uint8_t newPercent, lastPercent, currentpage, prevPage = 0;
  uint8_t dotDistance = 10;
  int8_t heaterStateDisplayed = 0;
  int8_t heaterModeDisplayed = 0;
  int8_t fuelLevelDisplayed = 0;
  double coolantTempDisplayed = 0.0;
  double exhaustTempDisplayed = 0.0;
  double voltageDisplayed = 9.0;

  int pixel_x, pixel_y;
  bool itemOneOn = false;
  bool itemGridOn = false;
  bool itemThreeOn = false;
  bool itemFourOn = true;
  bool itemFiveOn = true;
  bool initalPageRender = false;

  char next[6] = "NEXT";
  char back[6] = "BACK";
  char text_1[7] = "text 1";
  char text_2[7] = "text 2";
  char on[4] = "ON";
  char off[5] = "OFF";
  char grid[5] = "Grid";

  const char *stateTitleMap[6] = {
      "Off",
      "Sleep",
      "Start up sequence",
      "Active",
      "Shutting down",
      "Restarting"};

  const char *operationMap[12] = {
      "Switch off",
      "Low voltage",
      "Pre-start",
      "Venting",
      "Priming",
      "Igniting",
      "Monitoring flame",
      "Idle",
      "Eco",
      "Cooling < 150",
      "Startup failure",
      "Flame out"};

  const char *filename = "logoV2.bmp";

  void drawScaleFuelLevel();
  void drawScaleVoltage();
  void drawScaleExhaust();
  void drawScaleCoolant();
  bool TouchGetXY(void);
  void drawOldBar(int oldPer);
  void drawButtonList(Adafruit_GFX_Button **pb);
  bool update_button(Adafruit_GFX_Button *b, bool down);
  void drawButtonState(int x, int y, int width, int height, int sz, int text_color, int box_color, char *msg);

public:
  Display(MCUFRIEND_kbv &tft, TouchScreen &ts);
  void initilize();
  void page_0(void);
  void page_1(CanBusReceiver &data);
  void page_2(void);
  void showmsgXY(int x, int y, int sz, const GFXfont *f, int color, const char *msg);
  void printInteger(int x, int y, int sz, const GFXfont *f, int color, const int number);
  void printDouble(int x, int y, int sz, const GFXfont *f, int color, const double number);
  bool updateButtonList();
  void drawOutputState(int index);
  Adafruit_GFX_Button getNextBtn();
  Adafruit_GFX_Button getBackBtn();
  Adafruit_GFX_Button getGridBtn();
  uint8_t getCurrentPage();
  void bmpDraw(const char *filename, int x, int y);
  uint16_t read16(File f);
  uint32_t read32(File f);
  void initilizeSD();
  void drawFuelLevel(CanBusReceiver &data);
  void drawVoltage(CanBusReceiver &data);
  void drawExhaustTemp(CanBusReceiver &data);
  void drawCoolantTemp(CanBusReceiver &data);
  void drawPixelDots(uint8_t dotDistance);
  void drawHeaterState(CanBusReceiver &data);
  void drawHeaterMode(CanBusReceiver &data);
  void drawHeateStats(CanBusReceiver &data);
};

#endif