#include <Arduino.h>
#include <iostream>
#include "decode.h"
#include <ClockDisplay.h>
#include <TimeLib.h>

const int NUM_SEGMENTS = 2;
const int PIXEL_PIN = 9;
ClockDisplay display(PIXEL_PIN, NUM_SEGMENTS);
const int ANTENNA_PIN = 20;

const int SIGNAL_LED_PIN = 13;
const int STATE_LED_PIN = 14;

const int MIN_FALLING_EDGE_TIME = 30;

void setup()
{
  Serial.begin(9600);
  while (!Serial && millis() > 15000)
  {
    delay(100);
  }
  Serial.println("Hello");
  display.clear();
  display.show();
  setTime(0,0,0,0,0,0);
}

void displayTime()
{
  auto t = now();
  int h = hour(t);
  int m = minute(t);
  int s = second(t);

  display.clear();
  display.setNumber(h / 10, 0);
  display.setNumber(h % 10, 1);
  display.setNumber(m / 10, 2);
  display.setNumber(m % 10, 3);
  display.setSeconds(s);
  display.show();
}

uint32_t t = millis();
bool state = false;
uint32_t pulse_start = 0;
uint32_t first = 0;
int n_received = 0;
uint64_t data = 0;
uint32_t last_high_time = millis();

uint32_t last_update = 0;
const uint32_t interval = 500;

void loop()
{
  t = millis();
  bool signal = digitalRead(ANTENNA_PIN);
  digitalWrite(SIGNAL_LED_PIN, signal);
  digitalWrite(STATE_LED_PIN, state);

  if (signal)
  {
    last_high_time = t;
  }
  if (!state && signal)
  {
    // rising edge
    if (first == 0)
    {
      first = t;
    }
    else
    {
      float seconds = 1e-3 * (t - first);

      if (t - pulse_start > 900)
      {

        if ((t - pulse_start) > 1500)
        {
          if (n_received >= 59)
          {
            if (decode(data) == 0){
              display.setSignalMissing(false);
            } else {
              display.setSignalMissing(true);
            }
          }
          data = 0;
          n_received = 0;
          Serial.println("New Minute");
        }

        pulse_start = t;
        state = signal;
      }
    }
  }
  else if (state && !signal)
  {
    int signal_off_duration = t - last_high_time;
    if (signal_off_duration > MIN_FALLING_EDGE_TIME)
    {
      uint32_t duration = t - pulse_start;
      if (duration > 70)
      {
        bool value = duration > 150;
        if (value)
        {
          data |= static_cast<uint64_t>(1) << n_received;
        }
        n_received++;
        Serial.printf("Bit %d: %d (%d)\n", n_received, value, duration);
        state = signal;
        last_high_time = t;
      }
    }
  }


  if (timeStatus() == timeSet && ((t - last_update) > interval))
  {
    displayTime();
    last_update = t;
  }
  // delay(10);
}
