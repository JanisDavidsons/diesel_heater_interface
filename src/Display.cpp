#include "Display.h"

Display::Display(MCUFRIEND_kbv &tft, TouchScreen &ts)
    : tft(tft),
      ts(ts)
{
    page_0_btn[0] = &buttonNext;
    page_0_btn[1] = &buttonTwo;
    page_0_btn[2] = NULL;

    page_1_btn[0] = &buttonNext;
    page_1_btn[1] = &buttonBack;
    page_1_btn[2] = NULL;

    page_2_btn[0] = &buttonGrid;
    page_2_btn[1] = &buttonBack;
    page_2_btn[2] = &buttonFour;
    page_2_btn[3] = &buttonNext;
    page_2_btn[4] = &buttonThree;
    page_2_btn[5] = &buttonOne;
    page_2_btn[6] = NULL;
}

void Display::initilize()
{
    uint16_t ID = tft.readID();

    tft.begin(ID);
    tft.setRotation(1); // landscape
    tft.fillScreen(BLACK);
    tft.cp437(true);
    Serial.print("Display size ");
    Serial.print(tft.width());
    Serial.print("x");
    Serial.println(tft.height());
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

void Display::drawScaleExhaust()
{
    tft.drawFastVLine(224, 20, 150, WHITE);
    tft.drawFastHLine(216, 20, 8, WHITE);
    tft.drawFastHLine(219, 57.5, 5, WHITE);
    tft.drawFastHLine(216, 95, 8, WHITE);
    tft.drawFastHLine(219, 132.5, 5, WHITE);
    tft.drawFastHLine(216, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(173, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "200*");
    showmsgXY(176, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "100*");
    showmsgXY(195, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "0*");
    showmsgXY(212, 15, 1, &FreeSmallFont, YELLOW, "EHAUST");
}

void Display::drawScaleCoolant()
{
    tft.drawFastVLine(311, 20, 150, WHITE);
    tft.drawFastHLine(303, 20, 8, WHITE);
    tft.drawFastHLine(306, 57.5, 5, WHITE);
    tft.drawFastHLine(303, 95, 8, WHITE);
    tft.drawFastHLine(306, 132.5, 5, WHITE);
    tft.drawFastHLine(303, 170, 8, WHITE);
    tft.setTextColor(WHITE);

    showmsgXY(260, 25, 1, &FreeSerifBoldItalic9pt7b, WHITE, "100*");
    showmsgXY(269, 100, 1, &FreeSerifBoldItalic9pt7b, WHITE, "40*");
    showmsgXY(269, 170, 1, &FreeSerifBoldItalic9pt7b, WHITE, "-10*");
    showmsgXY(299, 15, 1, &FreeSmallFont, YELLOW, "COOLANT");
}

void Display::showmsgXY(int x, int y, int sz, const GFXfont *f, int color,
                        const char *msg)
{
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
    bmpDraw(filename, 20, 30);
    buttonNext.initButton(&tft, 342, 220, 120, 40, WHITE, CYAN, BLACK, next, 2);
    buttonTwo.initButton(&tft, 60, 220, 120, 40, WHITE, CYAN, BLACK, text_1, 2);
    drawButtonList(page_0_btn);
    drawPixelDots(dotDistance);
    tft.drawPixel(400, 240, RED);
}

void Display::page_1(CanBusReceiver &data)
{
    initalPageRender = true;
    currentpage = 1;
    tft.fillScreen(BLACK);

    buttonBack.initButton(&tft, 40, 220, 70, 40, WHITE, CYAN, BLACK, back, 2);
    buttonNext.initButton(&tft, 362, 220, 70, 40, WHITE, CYAN, BLACK, next, 2);

    drawButtonList(page_1_btn);

    drawScale_1();
    drawScale_2();
    drawScaleExhaust();
    drawScaleCoolant();
    drawCoolantTemp(data);
    drawExhaustTemp(data);
    drawHeaterState(data);
    drawHeaterMode(data);
    drawPixelDots(dotDistance);
    initalPageRender = false;
}

void Display::page_2(void)
{
    tft.fillScreen(BLACK);
    currentpage = 2;
    buttonGrid.initButton(&tft, 100, 20, 180, 40, WHITE, CYAN, BLACK, grid, 2);
    buttonFour.initButton(&tft, 100, 70, 180, 40, WHITE, CYAN, BLACK, text_2, 2);
    buttonThree.initButton(&tft, 100, 120, 180, 40, WHITE, CYAN, BLACK, text_1, 2);
    buttonOne.initButton(&tft, 100, 170, 180, 40, WHITE, CYAN, BLACK, text_2, 2);
    buttonNext.initButton(&tft, 300, 220, 180, 40, WHITE, CYAN, BLACK, next, 2);
    buttonBack.initButton(&tft, 100, 220, 180, 40, WHITE, CYAN, BLACK, back, 2);

    drawOutputState(0);
    drawButtonList(page_2_btn);
    drawPixelDots(dotDistance);
}

void Display::drawExhaustTemp(CanBusReceiver &data)
{
    bool isOutdated = data.getExhaustTemp().isOutdated;
    bool isTwoDigits = data.getExhaustTemp().value < 100;

    double exhaustTmp = data.getExhaustTemp().value;

    if (exhaustTmp == exhaustTempDisplayed && !initalPageRender)
    {
        return;
    }

    tft.fillRect(220, 175, 35, 20, BLACK);

    isOutdated ? showmsgXY(223, 200, 1, &FreeBigFont, RED, "--") : printInteger((isTwoDigits ? 233 : 224), 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, exhaustTmp);

    showmsgXY(255, 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, "*C");

    int new_temp = map(exhaustTmp, 0, 200, 0, 150);
    int last_temp = map(exhaustTempDisplayed, 0, 200, 0, 150);
    if (exhaustTmp < exhaustTempDisplayed)
    {
        tft.fillRect(228, 170 - last_temp, 30, last_temp - new_temp, BLACK);
    }
    else
    {
        tft.fillRect(228, 170 - new_temp, 30, new_temp - ((initalPageRender || isOutdated) ? 0 : last_temp), GREEN);
    }

    exhaustTempDisplayed = exhaustTmp;
}

void Display::drawCoolantTemp(CanBusReceiver &data)
{
    bool isOutdated = data.getHeaterTemp().isOutdated;
    bool isNineBelowFreezing = data.getHeaterTemp().coolant < -9;
    double coolantTmp = data.getCoolantTmp();

    if (coolantTmp == coolantTempDisplayed && !initalPageRender)
    {
        return;
    }

    tft.fillRect(320, 175, 40, 20, BLACK);

    isOutdated ? showmsgXY(320, 200, 1, &FreeBigFont, RED, "--") : printDouble((isNineBelowFreezing ? 315 : 320), 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, coolantTmp);

    showmsgXY(355, 190, 1, &FreeSerifBoldItalic9pt7b, GREEN, "*C");

    int new_temp = map(coolantTmp, -10, 100, 0, 150);
    int last_temp = map(coolantTempDisplayed, -10, 100, 0, 150);
    if (coolantTmp < coolantTempDisplayed)
    {
        tft.fillRect(315, 170 - last_temp, 30, last_temp - new_temp, BLACK);
    }
    else
    {
        tft.fillRect(315, 170 - new_temp, 30, new_temp - ((initalPageRender || isOutdated) ? 0 : last_temp), GREEN);
    }

    coolantTempDisplayed = coolantTmp;
}

bool Display::updateButtonList()
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

void Display::drawButtonState(int x, int y, int width, int height, int sz,
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

void Display::drawButtonList(Adafruit_GFX_Button **pb)
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

void Display::drawOutputState(int index)
{

    switch (index)
    {
    case 0:
        if (itemGridOn)
        {
            drawButtonState(200, 0, 70, 40, 2, BLACK, GREEN, on);
        }
        else
        {
            drawButtonState(200, 0, 70, 40, 2, BLACK, GRAY, off);
        }

        if (itemFiveOn)
        {
            drawButtonState(200, 150, 70, 40, 2, BLACK, GREEN, on);
        }
        else
        {
            drawButtonState(200, 150, 70, 40, 2, BLACK, GRAY, off);
        }

        if (itemFourOn)
        {
            drawButtonState(200, 50, 70, 40, 2, BLACK, GREEN, on); // plant light
        }
        else
        {
            drawButtonState(200, 50, 70, 40, 2, BLACK, GRAY, off);
        }

        if (itemThreeOn)
        {
            drawButtonState(200, 100, 70, 40, 2, BLACK, GREEN, on); // led light
        }
        else
        {
            drawButtonState(200, 100, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 1:
        itemGridOn = !itemGridOn;
        if (itemGridOn)
        {
            drawButtonState(200, 0, 70, 40, 2, BLACK, GREEN, on);
            drawPixelDots(dotDistance);
        }
        else
        {
            drawButtonState(200, 0, 70, 40, 2, BLACK, GRAY, off);
            page_2();
        }
        break;
    case 2:
        if (itemFiveOn)
        {
            drawButtonState(200, 150, 70, 40, 2, BLACK, GREEN, on);
        }
        else
        {
            drawButtonState(200, 150, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 3:
        if (!itemFourOn)
        {
            drawButtonState(200, 50, 70, 40, 2, BLACK, GREEN, on); // plant light from timer change
        }
        else
        {
            drawButtonState(200, 50, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 4:
        if (!itemThreeOn)
        {
            drawButtonState(200, 100, 70, 40, 2, BLACK, GREEN, on); // led light from timer change
        }
        else
        {
            drawButtonState(200, 100, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 5:
        if (itemFourOn)
        {
            drawButtonState(200, 50, 70, 40, 2, BLACK, GREEN, on); // plant light from button press
        }
        else
        {
            drawButtonState(200, 50, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 6:
        if (itemThreeOn)
        {
            drawButtonState(200, 100, 70, 40, 2, BLACK, GREEN, on); // led light from button press
        }
        else
        {
            drawButtonState(200, 100, 70, 40, 2, BLACK, GRAY, off);
        }
        break;
    case 7:
        //! water_pump_state ? draw_button_state(130, 200, 70, 40, 2, BLACK, GREEN, "ON"):tft.fillRect(130, 200, 70, 40, BLACK);

        if (itemOneOn)
        {
            drawButtonState(130, 200, 70, 40, 2, BLACK, GREEN, on); // led light from button press
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
    return buttonNext;
}

Adafruit_GFX_Button Display::getBackBtn()
{
    return buttonBack;
}

Adafruit_GFX_Button Display::getGridBtn()
{
    return buttonGrid;
}

uint8_t Display::getCurrentPage()
{
    return currentpage;
}

void Display::bmpDraw(const char *filename, int x, int y)
{
    initilizeSD();

    File bmpFile;
    int bmpWidth, bmpHeight;            // W+H in pixels
    uint8_t bmpDepth;                   // Bit depth (currently must be 24)
    uint32_t bmpImageoffset;            // Start of image data in file
    uint32_t rowSize;                   // Not always = bmpWidth; may have padding
    uint8_t sdbuffer[3 * BUFFPIXEL];    // pixel in buffer (R+G+B per pixel)
    uint16_t lcdbuffer[BUFFPIXEL];      // pixel out buffer (16-bit per pixel)
    uint8_t buffidx = sizeof(sdbuffer); // Current position in sdbuffer
    boolean goodBmp = false;            // Set to true on valid header parse
    boolean flip = true;                // BMP is stored bottom-to-top
    int w, h, row, col;
    uint8_t r, g, b;
    uint32_t pos = 0, startTime = millis();
    uint8_t lcdidx = 0;
    boolean first = true;

    if ((x >= tft.width()) || (y >= tft.height()))
        return;

    Serial.println();
    Serial.println("Loading image '");
    Serial.print(filename);
    Serial.println('\'');
    // Open requested file on SD card

    bmpFile = SD.open(filename);

    if (!bmpFile)
    {
        Serial.println("File not found");
        return;
    }

    // Parse BMP header
    if (read16(bmpFile) == 0x4D42)
    { // BMP signature
        Serial.println("File size: ");
        Serial.println(read32(bmpFile));
        (void)read32(bmpFile);            // Read & ignore creator bytes
        bmpImageoffset = read32(bmpFile); // Start of image data
        Serial.println("Image Offset: ");
        Serial.println(bmpImageoffset, DEC);
        // Read DIB header
        Serial.println("Header size: ");
        Serial.println(read32(bmpFile));
        bmpWidth = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        if (read16(bmpFile) == 1)
        {                               // # planes -- must be '1'
            bmpDepth = read16(bmpFile); // bits per pixel
            Serial.println("Bit Depth: ");
            Serial.println(bmpDepth);
            if ((bmpDepth == 24) && (read32(bmpFile) == 0))
            { // 0 = uncompressed

                goodBmp = true; // Supported BMP format -- proceed!
                Serial.println("Image size: ");
                Serial.print(bmpWidth);
                Serial.print('x');
                Serial.println(bmpHeight);

                // BMP rows are padded (if needed) to 4-byte boundary
                rowSize = (bmpWidth * 3 + 3) & ~3;

                // If bmpHeight is negative, image is in top-down order.
                // This is not canon but has been observed in the wild.
                if (bmpHeight < 0)
                {
                    bmpHeight = -bmpHeight;
                    flip = false;
                }

                // Crop area to be loaded
                w = bmpWidth;
                h = bmpHeight;
                if ((x + w - 1) >= tft.width())
                    w = tft.width() - x;
                if ((y + h - 1) >= tft.height())
                    h = tft.height() - y;

                // Set TFT address window to clipped image bounds
                tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

                for (row = 0; row < h; row++)
                { // For each scanline...
                    // Seek to start of scan line.  It might seem labor-
                    // intensive to be doing this on every line, but this
                    // method covers a lot of gritty details like cropping
                    // and scanline padding.  Also, the seek only takes
                    // place if the file position actually needs to change
                    // (avoids a lot of cluster math in SD library).
                    if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else // Bitmap is stored top-to-bottom
                        pos = bmpImageoffset + row * rowSize;
                    if (bmpFile.position() != pos)
                    { // Need seek?
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer); // Force buffer reload
                    }

                    for (col = 0; col < w; col++)
                    { // For each column...
                        // Time to read more pixel data?
                        if (buffidx >= sizeof(sdbuffer))
                        { // Indeed
                            // Push LCD buffer to the display first
                            if (lcdidx > 0)
                            {
                                tft.pushColors(lcdbuffer, lcdidx, first);
                                lcdidx = 0;
                                first = false;
                            }
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0; // Set index to beginning
                        }

                        // Convert pixel from BMP to TFT format
                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        lcdbuffer[lcdidx++] = tft.color565(r, g, b);
                    }
                }
                // Write any remaining data to LCD
                if (lcdidx > 0)
                {
                    tft.pushColors(lcdbuffer, lcdidx, first);
                }
                Serial.println("Loaded in ");
                Serial.print(millis() - startTime);
                Serial.println(" ms");
            }
        }
    }

    bmpFile.close();
    SD.end();
    if (!goodBmp)
        Serial.println("BMP format not recognized.");
}

uint16_t Display::read16(File f)
{
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t Display::read32(File f)
{
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

void Display::initilizeSD()
{
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    if (!SD.begin(SD_CS))
    {
        Serial.println("SD failed!");
    }
    else
    {
        Serial.println("SD initilized!");
    }
}

void Display::drawPixelDots(uint8_t dotDistance)
{
    if (itemGridOn)
    {
        for (int x = (tft.width() - 1); x >= 0; x -= dotDistance)
        {
            for (int y = (tft.height() - 1); y >= 0; y -= dotDistance)
            {
                tft.drawPixel(x, y, WHITE);
            }
        }
    }
}

void Display::drawHeaterState(CanBusReceiver &data)
{
    int8_t state = data.getHeateState().state;

    if (state == heaterStateDisplayed && !initalPageRender)
    {
        return;
    }
    tft.fillRect(81, 201, 238, 19, state == -1 ? BLACK : TEXT_BACKGROUND);
    tft.drawRect(80, 200, 240, 21, WHITE);

    state == -1 ? showmsgXY(85, 214, 0, &FreeMonoBold9pt7b, RED, "CAN-BUS failed.") : showmsgXY(85, 214, 0, &FreeMonoBold9pt7b, YELLOW, stateTitleMap[state]);
    heaterStateDisplayed = state;
}

void Display::drawHeaterMode(CanBusReceiver &data)
{
    int8_t mode = data.getHeateState().mode;

    if (mode == heaterModeDisplayed && !initalPageRender)
    {
        return;
    }
    tft.fillRect(81, 221, 238, 18, mode == -1 ? BLACK : TEXT_BACKGROUND);
    tft.drawRect(80, 220, 240, 20, WHITE);

    mode == -1 ? showmsgXY(85, 234, 0, &FreeMonoBold9pt7b, RED, "No message received.") : showmsgXY(85, 234, 0, &FreeMonoBold9pt7b, YELLOW, operationMap[mode]);
    heaterModeDisplayed = mode;
}
