#include "ClockDisplay.h"

#include <Adafruit_NeoPixel.h>
#include <Digits.h>

// 5x3 Matrizes of all digits
// 0-9 and colon

#include <cstdint>


ClockDisplay::ClockDisplay(int pin, int numSegments) {
    neopixel = Adafruit_NeoPixel(numSegments*segmentLeds, pin, NEO_GRBW + NEO_KHZ800);
    neopixel.begin();
    neopixel.setBrightness(10);
    neopixel.clear();
    neopixel.show();
}

int ClockDisplay::getDisplayPosition(int x, int y) {
    int segment = x / segmentWidth;  // Number of Segment
    int segmentX = x % segmentWidth; // X Position in Segment
    int segmentY = y;                // Y Position in Segment
    return segment * segmentLeds + segmentY * segmentWidth + segmentX;
};

void ClockDisplay::setNumber(int number, int position) {
    const auto& digit = digits[number];

    int digitWidth = 3;
    int digitHeight = 5;

    for (int i = 0; i < digitWidth; i++)
    {
        for (int j = 0; j < digitHeight; j++)
        {
            int displayPosition = getDisplayPosition(i+position*4+1, j);

            if (digit[j][i] == 1)
            {
                
                neopixel.setPixelColor(displayPosition, neopixel.Color(0, 0, 0, 255));
            }
            else
            {
                neopixel.setPixelColor(displayPosition, neopixel.Color(0, 0, 0, 0));
            }
        }
    }
}

void ClockDisplay::setSeconds(int seconds) {
    seconds += 2;
    int litLeds = seconds / 2;

    for (int i = 0; i < 30; i++)
    {
        if (i < litLeds)
        {
            if (i+1 == litLeds)
            {
                neopixel.setPixelColor(getDisplayPosition(1+i/2, (i%2)+6), neopixel.Color(0, 255*((seconds+1)%2), 0, 255*(seconds%2)));
            }
            else
            {
                neopixel.setPixelColor(getDisplayPosition(1+i/2, (i%2)+6), neopixel.Color(0, 0, 0, 255));
            }
        }
        else
        {
            neopixel.setPixelColor(getDisplayPosition(1+i/2, (i%2)+6), neopixel.Color(0, 0, 0, 0));
        }
    }

    if (signalMissing) {
        neopixel.setPixelColor(getDisplayPosition(0, 6), neopixel.Color(0, 0, seconds % 2 == 0 ? 255 : 0, 0));
        neopixel.setPixelColor(getDisplayPosition(0, 7), neopixel.Color(0, 0, seconds % 2 == 1 ? 255 : 0, 0));
    }
}

void ClockDisplay::setSignalMissing(bool signalMissing) {
    this->signalMissing = signalMissing;
}

void ClockDisplay::clear() {
    neopixel.clear();
}

void ClockDisplay::show() {
    neopixel.show();
}