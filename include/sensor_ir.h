#pragma once
#include "config.h"
#include <math.h>

// This file contains the active logic for the IR sensor.
// The rest of the sensors remain temporarily commented out in the project.

inline void initIR() {
  // Analog pins remain as INPUT by default.
}

inline void readIR() {
  // Average 16 readings before applying the datasheet curve approximation.
  uint32_t sum = 0;
  for (int i = 0; i < 16; ++i) {
    sum += analogReadMilliVolts(PIN_IR);
    delayMicroseconds(150);
  }
  uint32_t mv = sum / 16;

  float voltage = mv / 1000.0f;
  float distanceCm = 12.08f * powf(voltage, -1.058f);

  if (distanceCm < 4.0f || distanceCm > 30.0f) {
    Serial.println("nan");
    return;
  }

  Serial.println(distanceCm, 1);
}
