#include "Display.h"

Display::Display(MCUFRIEND_kbv &tft, TouchScreen &ts)
    : tft(tft),
      ts(ts)
{
    page_0_btn[0] = &nextBtn;
    page_0_btn[1] = &feed_btn;
    page_0_btn[2] = NULL;

    page_1_btn[0] = &nextBtn;
    page_1_btn[1] = &backBtn;
    page_1_btn[2] = NULL;

    page_2_btn[0] = &pumpBtn;
    page_2_btn[1] = &backBtn;
    page_2_btn[2] = &plant_light_on_btn;
    page_2_btn[3] = &nextBtn;
    page_2_btn[4] = &led_btn_on;
    page_2_btn[5] = &timer_btn_on;
    page_2_btn[6] = NULL;
}

void Display::drawScale_1()
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

void Display::drawScale_2()
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

void Display::drawScale_3()
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

void Display::drawScale_4()
{
    tft.drawFastVLine(311, 20, 150, WHITE);
    tft.drawFastHLine(303, 20, 8, WHITE);
    tft.drawFastHLine(306, 57.5, 5, WHITE);
    tft.drawFastHLine(303, 95, 8, WHITE);
    tft.drawFastHLine(306, 132.5, 5, WHITE);
    tft.drawFastHLine(303, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(269, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "50*");
    showmsgXY(269, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "25*");
    showmsgXY(269, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "0*");
    showmsgXY(299, 15, 1, &FreeSmallFont, YELLOW, "EXHAUST");
}

void Display::showmsgXY(int x, int y, int sz, const GFXfont *f, int color,
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

bool Display::TouchGetXY(void)
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

void Display::page_0(void)
{
    tft.fillScreen(BLACK);
    currentpage = 0;
    nextBtn.initButton(&tft, 342, 220, 120, 40, WHITE, CYAN, BLACK, next, 2);
    feed_btn.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, text_1, 2);
    draw_button_list(page_0_btn);
}

void Display::page_1(CanBusReceiver &data)
{
    tft.fillScreen(BLACK);
    initalPageRender = true;
    currentpage = 1;

    backBtn.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, back, 2);
    nextBtn.initButton(&tft, 342, 220, 120, 40, WHITE, CYAN, BLACK, next, 2);

    draw_button_list(page_1_btn);

    drawScale_1();
    drawScale_2();
    drawScale_3();
    drawScale_4();
    draw_water_temp(data);
    initalPageRender = false;
}

void Display::page_2(void)
{
    tft.fillScreen(BLACK);
    currentpage = 2;
    pumpBtn.initButton(&tft, 100, 20, 180, 40, WHITE, CYAN, BLACK, text_1, 2);
    plant_light_on_btn.initButton(&tft, 100, 70, 180, 40, WHITE, CYAN, BLACK, text_2, 2);
    led_btn_on.initButton(&tft, 100, 120, 180, 40, WHITE, CYAN, BLACK, text_1, 2);
    timer_btn_on.initButton(&tft, 100, 170, 180, 40, WHITE, CYAN, BLACK, text_2, 2);
    nextBtn.initButton(&tft, 300, 220, 180, 40, WHITE, CYAN, BLACK, next, 2);
    backBtn.initButton(&tft, 100, 220, 180, 40, WHITE, CYAN, BLACK, back, 2);

    // Drawing position of the buttons on the screen
    draw_output_state(0);

    draw_button_list(page_2_btn);
}

void Display::draw_water_temp(CanBusReceiver &data)
{
    double coolantTmp = data.getCoolantTmp();

    if (coolantTmp == coolantTempDisplayed && !initalPageRender)
    {
        return;
    }

    tft.fillRect(320, 175, 40, 20, BLACK);
    printDouble(320, 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, coolantTmp);
    showmsgXY(355, 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, "*C");

    int new_temp = map(coolantTmp, 0, 50, 0, 150);
    int last_temp = map(coolantTempDisplayed, 0, 50, 0, 150);
    if (coolantTmp < coolantTempDisplayed)
    {
        tft.fillRect(315, 170 - last_temp, 30, last_temp - new_temp, BLACK);
    }
    else
    {
        tft.fillRect(315, 170 - new_temp, 30, new_temp - (initalPageRender ? 0 : last_temp), GREEN);
    }

    coolantTempDisplayed = coolantTmp;
}

bool Display::update_button_list()
{
    bool down = TouchGetXY();
    Adafruit_GFX_Button **pb;

    switch (currentpage)
    {
    case 0:
        pb = page_0_btn;
        break;
    case 1:
        pb = page_1_btn;
        break;
    case 2:
        pb = page_2_btn;
        break;
    default:
        Serial.println("Error, unsoported page set!");
        return false;
    }

    for (int i = 0; pb[i] != NULL; i++)
    {
        update_button(pb[i], down);
    }
    return down;
}

void Display::printInteger(int x, int y, int sz, const GFXfont *f, int color, const int number)
{
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);
    tft.print(number);
    tft.setFont(NULL);
}

void Display::printDouble(int x, int y, int sz, const GFXfont *f, int color, const double number)
{
    int intValue = (int)(number * 10); // Multiply by 10 to keep one decimal place
    int wholePart = intValue / 10;
    int decimalPart = intValue % 10;
    tft.setFont(f);
    tft.setCursor(x, y);
    tft.setTextColor(color);
    tft.setTextSize(sz);

    tft.print(wholePart);
    tft.print('.');
    tft.print(decimalPart);
    tft.setFont(NULL);
}

void Display::draw_button_state(int x, int y, int width, int height, int sz,
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

void Display::draw_button_list(Adafruit_GFX_Button **pb)
{
    for (int i = 0; pb[i] != NULL; i++)
    {
        pb[i]->drawButton(false);
    }
}

bool Display::update_button(Adafruit_GFX_Button *b, bool down)
{
    b->press(down && b->contains(pixel_x, pixel_y));
    if (b->justReleased())
        b->drawButton(false);
    if (b->justPressed())
        b->drawButton(true);
    return down;
}

void Display::draw_output_state(int index)
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

Adafruit_GFX_Button Display::getNextBtn()
{
    return nextBtn;
}

Adafruit_GFX_Button Display::getBackBtn()
{
    return backBtn;
}

Adafruit_GFX_Button Display::getPumpBtn()
{
    return pumpBtn;
}

uint8_t Display::getCurrentPage()
{
    return currentpage;
}