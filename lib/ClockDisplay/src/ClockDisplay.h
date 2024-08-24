#include <Adafruit_NeoPixel.h>


class ClockDisplay {
    public:
        ClockDisplay(int pin, int numSegments);
        Adafruit_NeoPixel neopixel;
        int getDisplayPosition(int x, int y);
        void setNumber(int number, int position);
        void clear();
        void show();
    private:
        int segmentLeds = 64;
        int segmentWidth = 8;
};
