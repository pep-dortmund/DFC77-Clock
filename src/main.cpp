#include <Arduino.h>
#include <ClockDisplay.h>
#include <Adafruit_NeoPixel.h>

#define PIN 9
#define DELAYVAL 500

ClockDisplay display(PIN, 2);
//Adafruit_NeoPixel neopixel(2*NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);

int hours;
int minutes;
int i = 0;

void setup() {
  hours = 18;
  minutes = 40;
}

void loop() {
  display.clear();
    i++;
    display.setNumber(0, 0);
    display.setNumber(hours / 10, 0);
    display.setNumber(hours % 10, 1);
    display.setNumber(minutes / 10, 2);
    display.setNumber(minutes % 10, 3);

    delay(1000);
    minutes++;
    if (minutes == 60) {
      minutes = 0;
      hours++;
      if (hours == 24) {
        hours = 0;
      }
    } 
    delay(60000);
}


// void loop() {
  
  
//   neopixel.clear();

//   int breite = 8;

// for (int lc = breite; lc > 0; lc--)
// {
//   for (int i = breite-lc; i < lc; i++) {
//     neopixel.setPixelColor(i+breite*(breite - lc), neopixel.Color(255, 0, 0));
//     neopixel.show();
//     delay(DELAYVAL);
//   }

//   for (int i = breite-lc; i < lc; i++) {
//     neopixel.setPixelColor((i+2)*breite-1-(breite -lc), neopixel.Color(0, 255, 0));
//     neopixel.show();
//     delay(DELAYVAL);
//   }

//   for (int i = lc; i > breite - lc; i--) {
//     neopixel.setPixelColor(breite*(breite-1)+i-1-breite*(breite - lc), neopixel.Color(0, 0, 255));
//     neopixel.show();
//     delay(DELAYVAL);
//   }

//   for (int i = lc; i > breite-lc; i--) {
//     neopixel.setPixelColor((i-2)*breite+(breite-lc), neopixel.Color(255, 255, 0));
//     neopixel.show();
//     delay(DELAYVAL);
//   }
// }


//   // Loop in a inward spiral


// }
