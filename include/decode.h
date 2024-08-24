#include <cstdint>
#include <iostream>
#undef B1
#include <fmt/core.h>
#include <TimeLib.h>
#include <fmt/compile.h>

bool get_bit(uint64_t data, int pos) {
    return data & (1ull << pos);
}

int get_int(uint64_t data, int pos, int length) {
    uint64_t filter = ((1ull << length) - 1) << pos;
    return (data & filter) >> pos;
}


bool parity(uint64_t data, int pos, int length) {
    int parity_number = 0;
    for (int i = 0; i < length; i++) {
        if (get_bit(data, pos + i)) {
            parity_number += 1;
        }
    }
    return parity_number % 2 == 0;
}


void decode(uint64_t data) {
  
    if (!parity(data, 21, 8)) {
        Serial.println("Parity check on minute failed");
        return;
    }
    if (!parity(data, 29, 7)) {
        Serial.println("Parity check on hour failed");
        return;
    }
    if (!parity(data, 36, 23)) {
        Serial.println("Parity check on date failed");
        return;
    }

    TimeElements tm;

    tm.Year = 30 +  get_int(data, 50, 4) + 10 * get_int(data, 54, 4);
    tm.Month = get_int(data, 45, 4) + 10 * get_int(data, 49, 1);
    tm.Day = get_int(data, 36, 4) + 10 * get_int(data, 40, 2);
    tm.Hour = get_int(data, 29, 4) + 10 * get_int(data, 33, 2);
    tm.Minute = get_int(data, 21, 4) + 10 * get_int(data, 25, 3);

    setTime(tm.Hour, tm.Minute, 0, tm.Day, tm.Month, tm.Year);

    std::string date = fmt::format(FMT_COMPILE("{:04d}-{:02d}-{:02d}T{:02d}:{:02d}"), tm.Year+1970, tm.Month, tm.Day, tm.Hour, tm.Minute);
    Serial.println(date.c_str());
}