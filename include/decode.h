#include <cstdint>
#include <iostream>
#undef B1
#include <fmt/core.h>
#include <TimeLib.h>
#include <fmt/compile.h>
#include <Arduino.h>
#include <stdexcept>

bool get_bit(uint64_t data, int pos)
{
    return data & (1ull << pos);
}

int get_int(uint64_t data, int pos, int length)
{
    uint64_t filter = ((1ull << length) - 1) << pos;
    return (data & filter) >> pos;
}

bool parity(uint64_t data, int pos, int length)
{
    int parity_number = 0;
    for (int i = 0; i < length; i++)
    {
        if (get_bit(data, pos + i))
        {
            parity_number += 1;
        }
    }
    return parity_number % 2 == 0;
}

int get_decimal(uint64_t data, int pos, int onelength, int tenlength)
{
    int one = get_int(data, pos, onelength);
    int ten = get_int(data, pos + onelength, tenlength);
    if (one >= 10)
    {
        return -1;
    }

    return one + 10 * ten;
}

int decode(uint64_t data)
{
    Serial.println(data, BIN);
    if (!parity(data, 21, 8))
    {
        Serial.println("Parity check on minute failed");
        return 1;
    }

    if (!parity(data, 29, 7))
    {
        Serial.println("Parity check on hour failed");
        return 1;
    }

    if (!parity(data, 36, 23))
    {
        Serial.println("Parity check on date failed");
        return 1;
    }

    TimeElements tm;

    tm.Year = 30 + get_decimal(data, 50, 4, 4);
    tm.Month = get_decimal(data, 45, 4, 1);
    if (tm.Month > 12 || tm.Month < 1)
    {
        Serial.println("Invalid month");
        return 1;
    }
    tm.Day = get_decimal(data, 36, 4, 2);
    if (tm.Day > 31 || tm.Day < 1)
    {
        Serial.println("Invalid day");
        return 1;
    }
    tm.Hour = get_decimal(data, 29, 4, 2);
    if (tm.Hour > 23 || tm.Hour < 0)
    {
        Serial.println("Invalid hour");
        return 1;
    }
    tm.Minute = get_decimal(data, 21, 4, 3);
    if (tm.Minute > 59 || tm.Minute < 0)
    {
        Serial.println("Invalid minute");
        return 1;
    }
    tm.Second = 0;

    setTime(tm.Hour, tm.Minute, 0, tm.Day, tm.Month, tm.Year);

    std::string date = fmt::format(FMT_COMPILE("{:04d}-{:02d}-{:02d}T{:02d}:{:02d}"), tm.Year + 1970, tm.Month, tm.Day, tm.Hour, tm.Minute);
    Serial.println(date.c_str());
    return 0;
}