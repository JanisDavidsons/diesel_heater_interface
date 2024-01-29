#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <time.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <arduino-timer.h>

#include "CanBusReceiver.h"
#include "RoomTemperature.h"
#include "Display.h"

class MCUFRIEND_kbv tft;

const int XP = 6, XM = A2, YP = A1, YM = 7;
const int TS_LEFT = 973, TS_RT = 187, TS_TOP = 247, TS_BOT = 803;

MCP2515 mcp2515(53);
CanBusReceiver canbus(mcp2515);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Display display(tft, ts);

auto timer = timer_create_default();
void runTimeCallback();
int freeMemory();

bool water_pump_state = false;

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

void setup()
{
    Serial.begin(9600);

    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS);
    mcp2515.setNormalMode();

    uint16_t ID = tft.readID();

    tft.begin(ID);
    tft.setRotation(1); // landscape
    tft.fillScreen(BLACK);
    tft.cp437(true);

    display.page_0();
    runTimeCallback();
}

void loop()
{
    timer.tick();
    canbus.checkMessage();
    display.update_button_list();
    uint8_t currentPage = display.getCurrentPage();

    if (currentPage == 0)
    {
        if (display.getNextBtn().justPressed())
        {
            display.page_1(canbus);
        }
    }
    else if (currentPage == 1)
    {
        display.draw_water_temp(canbus);

        if (display.getBackBtn().justPressed())
        {
            display.page_0();
        }
        else if (display.getNextBtn().justPressed())
        {
            display.page_2();
        }
    }
    else if (currentPage == 2)
    {
        if (display.getBackBtn().justPressed())
        {
            display.page_1(canbus);
        }
        else if (display.getPumpBtn().justPressed())
        {
            display.draw_output_state(1);
            water_pump_state = !water_pump_state;
        }
        else if (display.getNextBtn().justPressed())
        {
            display.page_2();
        }
    }
}

void runTimeCallback()
{
    timer.every(
        1000,
        [](void *) -> bool
        {
            // Serial.println(water_temp = canbus.getExhaustTemp());
            // Print room temperature every 5 seconds, will be sent to the heater later on.

            // Serial.print("Room temperature: ");
            // Serial.println(temperature.readTemperature());
            return true;
        });

    timer.every(
        500,
        [](void *) -> bool
        {
            // Serial.print("Free memory: ");
            // Serial.println(freeMemory());
            return true;
        });
}

int freeMemory()
{
    int free_memory;
    if ((int)__brkval == 0)
        free_memory = ((int)&free_memory) - ((int)&__heap_start);
    else
        free_memory = ((int)&free_memory) - ((int)__brkval);
    return free_memory;
}