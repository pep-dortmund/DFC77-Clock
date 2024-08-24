#include <Arduino.h>
#include <iostream>
#include "decode.h"
#include <ClockDisplay.h>
#include <TimeLib.h>

const int NUM_SEGMENTS = 2;
const int PIXEL_PIN = 9;
ClockDisplay display(PIXEL_PIN, NUM_SEGMENTS);
const int ANTENNA_PIN = 20;

const int led_pin = 13;

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
}

bool state = false;
uint32_t pulse_start = 0;
uint32_t first = 0;
int n_received = 0;
uint64_t data = 0;

void displayTime()
{
  auto t = now();
  int h = hour(t);
  int m = minute(t);
  int s = second(t);

  display.setNumber(h / 10, 0);
  display.setNumber(h % 10, 1);
  display.setNumber(m / 10, 2);
  display.setNumber(m % 10, 3);
  display.show();
}

uint32_t last_update = 0;
const uint32_t interval = 500;

void loop()
{
  uint32_t t = millis();
  bool signal = digitalRead(ANTENNA_PIN);
  digitalWrite(led_pin, signal);

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
      Serial.println(seconds, 1);

      if ((t - pulse_start) > 1500)
      {
        if (n_received >= 59)
        {
          decode(data);
        }
        data = 0;
        n_received = 0;
        Serial.println("New Minute");
      }
    }
    pulse_start = t;
  }
  else if (state && !signal)
  {
    // falling edge
    uint32_t duration = t - pulse_start;
    bool value = duration > 150;
    if (value)
    {
      data |= static_cast<uint64_t>(1) << n_received;
    }
    n_received++;
    Serial.println(duration);
  }

  state = signal;

  if (timeStatus() == timeSet && ((t - last_update) > interval))
  {
    Serial.println("Update display");
    displayTime();
    last_update = t;
  }
  // delay(10);
}
