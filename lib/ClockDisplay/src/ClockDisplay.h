#include <Adafruit_NeoPixel.h>


class ClockDisplay {
    public:
        ClockDisplay(int pin, int numSegments);
        Adafruit_NeoPixel neopixel;
        int getDisplayPosition(int x, int y);
        void setNumber(int number, int position);
        void setSeconds(int seconds);
        void setSignalMissing(bool signalMissing);
        void clear();
        void show();
    private:
        int segmentLeds = 64;
        int segmentWidth = 8;
        bool signalMissing = true;
};
