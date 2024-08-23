#include <Adafruit_NeoPixel.h>
#include <Digits.h>

// 5x3 Matrizes of all digits
// 0-9 and colon

#include <cstdint>

class ClockDisplay
{
    int segmentLeds = 64;
    int segmentWidth = 8;

public:
    Adafruit_NeoPixel neopixel;

    ClockDisplay(int pin, int numSegments);
    int getDisplayPosition(int x, int y);
    void setNumber(int number, int position);
    void clear();
};

ClockDisplay::ClockDisplay(int pin, int numSegments) : neopixel(numSegments*segmentLeds, pin, NEO_GRBW + NEO_KHZ800)
{
        neopixel.setBrightness(1);
        neopixel.begin();
        neopixel.clear();
        neopixel.show();
}

int ClockDisplay::getDisplayPosition(int x, int y)
    {
        int segment = x / segmentWidth;  // Number of Segment
        int segmentX = x % segmentWidth; // X Position in Segment
        int segmentY = y;                // Y Position in Segment
        return segment * segmentLeds + segmentY * segmentWidth + segmentX;
    };

void ClockDisplay::setNumber(int number, int position)
    {
        const auto& digit = digits[number];

        int digitWidth = 3;
        int digitHeight = 5;

        for (int i = 0; i < digitWidth; i++)
        {
            for (int j = 0; j < digitHeight; j++)
            {
                int displayPosition = getDisplayPosition(i+position*4+1, j+1);

                if (digit[j][i] == 1)
                {
                    neopixel.setPixelColor(displayPosition, neopixel.Color(255, 255, 255, 255));
                }
                else
                {
                    neopixel.setPixelColor(displayPosition, neopixel.Color(0, 0, 0, 0));
                }
            }
        }

        neopixel.show();
    }

void ClockDisplay::clear()
    {
        neopixel.clear();
    }