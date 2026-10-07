#pragma once
#include "config.h"

inline void initTemp() {
  // For the LM35, no extra pin configuration is needed.
  // Analog reading is done with analogReadMilliVolts().
}

inline void readTemp() {
  // The LM35 output is 10 mV per °C and 0 V at 0 °C.
  // Therefore the temperature in °C is directly: mv / 10.0.
  uint32_t mv = analogReadMilliVolts(PIN_TEMP);
  float tempC = mv / 10.0f;

  //Serial.print("temp ");
  Serial.println(tempC, 2);
}
