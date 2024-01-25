#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <FreeDefaultFonts.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>

class MCUFRIEND_kbv tft;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

#define BLACK 0x0000
#define WHITE 0xFFFF
#define CYAN 0x07FF
#define YELLOW 0xFFE0
#define GREEN 0x07E0
#define GRAY 0x8410

#define SHOW_COORDINATES_ON false // change this to view touch point coordinates in serial monitor

Adafruit_GFX_Button
    plant_light_on_btn,
    next_btn,
    feed_btn,
    start_btn,
    temp_btn,
    level_btn,
    back_btn,
    water_pump_on_btn,
    led_btn_on,
    timer_btn_on;

const int XP = 6, XM = A2, YP = A1, YM = 7;
const int TS_LEFT = 973, TS_RT = 187, TS_TOP = 247, TS_BOT = 803; // changed to landscape

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button *page_0_btn[] = {&next_btn, &feed_btn, NULL};
Adafruit_GFX_Button *page_1_btn[] = {&next_btn, &back_btn, NULL};
Adafruit_GFX_Button *page_2_btn[] = {
    &water_pump_on_btn,
    &back_btn,
    &plant_light_on_btn,
    &next_btn,
    &led_btn_on,
    &timer_btn_on,
    NULL};

void showmsgXY(int, int, int, const GFXfont *f, int, const char *msg);
void draw_button_state(int, int, int, int, int, int, int, char *msg);
void draw_button_list(class Adafruit_GFX_Button *page_n_buttons[]);
bool update_button_list(class Adafruit_GFX_Button *home_btn[]);
void draw_output_state(int);
void draw_water_temp(bool hasTempChanged = false);
void drawScale_1();
void drawScale_2();
void drawScale_3();
void drawScale_4();

uint8_t newPercent, lastPercent, currentpage, prevPage = 0;

int pixel_x, pixel_y; // Touch_getXY() updates global vars
float last_water_temp = 20;
float water_temp = 23;
unsigned long currentSeconds = 0;
unsigned long previousSeconds = 0;
int feedTimerCounter = 0;
bool feed_timer_on = false;
bool water_pump_state = false;
bool led_relay_state = false;
bool plant_light_state = true;
bool timer_state = false;
bool plant_light_on = false;
bool led_light_on = false;
bool led_driver_on = false;
bool timer_on = true;

char next[] = "NEXT ";
char back[] = "BACK ";
char text_1[] = "text 1";
char text_2[] = "text 2";
char on[] = "ON";
char off[] = "OFF";

void drawScale_1()
{

    tft.drawFastVLine(50, 20, 150, WHITE);
    tft.drawFastHLine(42, 20, 8, WHITE);
    tft.drawFastHLine(45, 57.5, 5, WHITE);
    tft.drawFastHLine(42, 95, 8, WHITE);
    tft.drawFastHLine(45, 132.5, 5, WHITE);
    tft.drawFastHLine(42, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(0, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "100%");
    showmsgXY(10, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "50%");
    showmsgXY(25, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "0%");
    showmsgXY(45, 15, 1, &FreeSmallFont, YELLOW, "LEVEL");
}

//===========================SECOND SCALE===================================================================

void drawScale_2()
{
    tft.drawFastVLine(137, 20, 150, WHITE);
    tft.drawFastHLine(129, 20, 8, WHITE);
    tft.drawFastHLine(132, 57.5, 5, WHITE);
    tft.drawFastHLine(129, 95, 8, WHITE);
    tft.drawFastHLine(132, 132.5, 5, WHITE);
    tft.drawFastHLine(129, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(95, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "50*");
    showmsgXY(95, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "25*");
    showmsgXY(105, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "0*");
    showmsgXY(125, 15, 1, &FreeSmallFont, YELLOW, "TEMP 1");
}

//===========================THIRD SCALE===================================================================

void drawScale_3()
{
    tft.drawFastVLine(224, 20, 150, WHITE);
    tft.drawFastHLine(216, 20, 8, WHITE);
    tft.drawFastHLine(219, 57.5, 5, WHITE);
    tft.drawFastHLine(216, 95, 8, WHITE);
    tft.drawFastHLine(219, 132.5, 5, WHITE);
    tft.drawFastHLine(216, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(182, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "50*");
    showmsgXY(185, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "25*");
    showmsgXY(195, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "0*");
    showmsgXY(212, 15, 1, &FreeSmallFont, YELLOW, "TEMP 2");
}

//===========================FOURTH SCALE===================================================================

void drawScale_4()
{
    tft.drawFastVLine(311, 20, 150, WHITE);
    tft.drawFastHLine(303, 20, 8, WHITE);
    tft.drawFastHLine(306, 57.5, 5, WHITE);
    tft.drawFastHLine(303, 95, 8, WHITE);
    tft.drawFastHLine(306, 132.5, 5, WHITE);
    tft.drawFastHLine(303, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(269, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "40*");
    showmsgXY(269, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "30*");
    showmsgXY(269, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "20*");
    showmsgXY(299, 15, 1, &FreeSmallFont, YELLOW, "WATER TEMP");
}

void page_0(void)
{
    tft.fillScreen(BLACK);
    currentpage = 0;
    next_btn.initButton(&tft, 342, 220, 120, 40, WHITE, CYAN, BLACK, next, 2);
    feed_btn.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, text_1, 2);
    draw_button_list(page_0_btn);
}

void page_1(void)
{
    tft.fillScreen(BLACK);
    currentpage = 1;

    back_btn.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, back, 2);
    next_btn.initButton(&tft, 342, 220, 120, 40, WHITE, CYAN, BLACK, next, 2);

    draw_button_list(page_1_btn);

    drawScale_1();
    drawScale_2();
    drawScale_3();
    drawScale_4();
    draw_water_temp();
}

void page_2(void)
{
    tft.fillScreen(BLACK);
    currentpage = 2;
    water_pump_on_btn.initButton(&tft, 100, 20, 180, 40, WHITE, CYAN, BLACK, text_1, 2);
    plant_light_on_btn.initButton(&tft, 100, 70, 180, 40, WHITE, CYAN, BLACK, text_2, 2);
    led_btn_on.initButton(&tft, 100, 120, 180, 40, WHITE, CYAN, BLACK, text_1, 2);
    timer_btn_on.initButton(&tft, 100, 170, 180, 40, WHITE, CYAN, BLACK, text_2, 2);
    next_btn.initButton(&tft, 300, 220, 180, 40, WHITE, CYAN, BLACK, next, 2);
    back_btn.initButton(&tft, 100, 220, 180, 40, WHITE, CYAN, BLACK, back, 2);

    // Drawing position of the buttons on the screen
    draw_output_state(0);

    draw_button_list(page_2_btn);
}

void setup()
{
    Serial.begin(9600);

    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3)
        ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(1); // landscape
    tft.fillScreen(BLACK);

    Serial.print(tft.width());
    Serial.print(F(" x "));
    Serial.println(tft.height());

    page_0();
}

void loop() {    
    if (currentpage == 0) {
        update_button_list(page_0_btn);
        if (next_btn.justPressed()) {
            page_1();
        }
    } else if (currentpage == 1) {
        update_button_list(page_1_btn);
        if (water_temp != last_water_temp) {
            draw_water_temp(true);
        }

        if (back_btn.justPressed()) {
            page_0();
        } else if (next_btn.justPressed()) {
            page_2();
        }
    } else if (currentpage == 2) {
        update_button_list(page_2_btn);
        if (back_btn.justPressed()) {
            page_1();
        } else if (water_pump_on_btn.justPressed()) {
            draw_output_state(1);
            water_pump_state = !water_pump_state;
        } else if (next_btn.justPressed()) {
            page_2();
        }
    }
}


//===========================Function returns,sets pressed X and Y coordinates=====================================

bool Touch_getXY(void)
{
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT); // restore shared pins
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH); // because TFT control pins
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed)
    {
        pixel_x = map(p.y, TS_LEFT, TS_RT, 0, 400); // changed to portrait
        pixel_y = map(p.x, TS_TOP, TS_BOT, 0, 240); // changed to portrait
        if (SHOW_COORDINATES_ON)
        {
            Serial.print("X = ");
            Serial.print(pixel_x); // if set true, prints touch coordinates on Serial.print
            Serial.print("\tY = ");
            Serial.println(pixel_y); // if set true, prints touch coordinates on Serial.print
        }
    }
    return pressed;
}

/* update the state of a button and redraw as required
 *
 * main program can use isPressed(), justPressed() etc
 */
bool update_button(Adafruit_GFX_Button *b, bool down)
{
    b->press(down && b->contains(pixel_x, pixel_y));
    if (b->justReleased())
        b->drawButton(false);
    if (b->justPressed())
        b->drawButton(true);
    return down;
}

/* most screens have different sets of buttons
 * life is easier if you process whole list in one go
 */
bool update_button_list(Adafruit_GFX_Button **pb)
{
    bool down = Touch_getXY();
    for (int i = 0; pb[i] != NULL; i++)
    {
        update_button(pb[i], down);
    }
    return down;
}

void draw_button_list(Adafruit_GFX_Button **pb)
{
    for (int i = 0; pb[i] != NULL; i++)
    {
        pb[i]->drawButton(false);
    }
}

//===============================Function to draw button state=====================================
void draw_button_state(int x, int y, int width, int height, int sz,
                       int text_color, int box_color, char *msg)
{
    tft.fillRect(x, y, width, height, box_color);
    tft.drawRect(x, y, width, height, WHITE);
    // tft.setFont(f);
    tft.setCursor(x + 22, y + 12);
    tft.setTextColor(BLACK);
    tft.setTextSize(sz);
    tft.print(msg);
    tft.setFont(NULL);
}

//===============================Function to draw characters on tft=====================================
void showmsgXY(int x, int y, int sz, const GFXfont *f, int color,
               const char *msg)
{
    // int16_t x1, y1;
    // uint16_t wid, ht;
    // tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);
    tft.print(msg);
    tft.setFont(NULL);
}

//===============================Function to draw numbers on tft=====================================
void showNumXY(int x, int y, int sz, const GFXfont *f, int color,
               const int number)
{
    // int16_t x1, y1;
    // uint16_t wid, ht;
    // tft.drawFastHLine(0, y, tft.width(), WHITE);
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);
    tft.print(number);
    tft.setFont(NULL);
}

//===========================Function to redraw previous bar =============================================
void drawOldBar(int oldPer)
{
    tft.fillRect(128, 20 + (150 - oldPer), 10, oldPer, GREEN);
}

//========================================================================================================

void draw_water_temp(bool hasTempChanged)
{
    tft.fillRect(320, 175, 40, 20, BLACK);
    showNumXY(320, 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, water_temp);
    showmsgXY(340, 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, "*C");

    int new_temp = map(water_temp, 20, 40, 0, 150);
    int last_temp = map(last_water_temp, 20, 40, 0, 150);
    if (water_temp < last_water_temp)
    {
        tft.fillRect(315, 170 - last_temp, 30, last_temp - new_temp, BLACK);
    }
    else
    {
        tft.fillRect(315, 170 - new_temp, 30, new_temp - (hasTempChanged? last_temp : 0), GREEN);
    }
    last_water_temp = water_temp;
}

void draw_output_state(int index)
{

    switch (index)
    {
    case 0:
        if (!water_pump_state)
        {
            draw_button_state(200, 0, 70, 40, 2, BLACK, GREEN, on); // water pump
        }
        else
        {
            draw_button_state(200, 0, 70, 40, 2, BLACK, GRAY, off);
        }

        if (timer_on)
        {
            draw_button_state(200, 150, 70, 40, 2, BLACK, GREEN, on); // timer
        }
        else
        {
            draw_button_state(200, 150, 70, 40, 2, BLACK, GRAY, off);
        }

        if (plant_light_state)
        {
            draw_button_state(200, 50, 70, 40, 2, BLACK, GREEN, on); // plant light
        }
        else
        {
            draw_button_state(200, 50, 70, 40, 2, BLACK, GRAY, off);
        }

        if (led_relay_state)
        {
            draw_button_state(200, 100, 70, 40, 2, BLACK, GREEN, on); // led light
        }
        else
        {
            draw_button_state(200, 100, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 1:
        if (water_pump_state)
        {
            draw_button_state(200, 0, 70, 40, 2, BLACK, GREEN, on); // water pump
        }
        else
        {
            draw_button_state(200, 0, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 2:
        if (timer_on)
        {
            draw_button_state(200, 150, 70, 40, 2, BLACK, GREEN, on); // timer
        }
        else
        {
            draw_button_state(200, 150, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 3:
        if (!plant_light_state)
        {
            draw_button_state(200, 50, 70, 40, 2, BLACK, GREEN, on); // plant light from timer change
        }
        else
        {
            draw_button_state(200, 50, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 4:
        if (!led_relay_state)
        {
            draw_button_state(200, 100, 70, 40, 2, BLACK, GREEN, on); // led light from timer change
        }
        else
        {
            draw_button_state(200, 100, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 5:
        if (plant_light_state)
        {
            draw_button_state(200, 50, 70, 40, 2, BLACK, GREEN, on); // plant light from button press
        }
        else
        {
            draw_button_state(200, 50, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 6:
        if (led_relay_state)
        {
            draw_button_state(200, 100, 70, 40, 2, BLACK, GREEN, on); // led light from button press
        }
        else
        {
            draw_button_state(200, 100, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 7:
        //! water_pump_state ? draw_button_state(130, 200, 70, 40, 2, BLACK, GREEN, "ON"):tft.fillRect(130, 200, 70, 40, BLACK);

        if (feed_timer_on)
        {
            draw_button_state(130, 200, 70, 40, 2, BLACK, GREEN, on); // led light from button press
        }
        else
        {
            tft.fillRect(130, 200, 70, 40, BLACK);
        }
        break;

    default:
        break;
    }
}