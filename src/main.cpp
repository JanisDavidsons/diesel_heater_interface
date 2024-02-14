#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <time.h>
#include <TouchScreen.h>
#include <MCUFRIEND_kbv.h>
#include <arduino-timer.h>

#include "CanBusReceiver.h"
#include "RoomTemperature.h"
#include "Display.h"

class MCUFRIEND_kbv tft;

const int XP=9,XM=A3,YP=A2,YM=8;

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
    display.initilize();

    // delay(2000);
    // display.page_0();
    display.page_1(canbus);

    runTimeCallback();
}

void loop()
{
    timer.tick();
    canbus.checkMessage();
    display.updateButtonList();
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
        display.drawFuelLevel(canbus);
        display.drawVoltage(canbus);
        display.drawCoolantTemp(canbus);
        display.drawExhaustTemp(canbus);
        display.drawHeaterState(canbus);
        display.drawHeaterMode(canbus);
        display.drawHeaterStats(canbus);

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
        else if (display.getGridBtn().justPressed())
        {
            display.drawOutputState(1);
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
            canbus.tick();
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