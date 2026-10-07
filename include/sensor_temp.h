#pragma once
#include "config.h"

inline void initTemp() {
  // Para el LM35, no hace falta configuración extra en el pin.
  // La lectura analógica se hace con analogReadMilliVolts().
}

inline void readTemp() {
  // La salida del LM35 es de 10 mV por °C y 0 V a 0 °C.
  // Por eso la temperatura en °C es directamente: mv / 10.0.
  uint32_t mv = analogReadMilliVolts(PIN_TEMP);
  float tempC = mv / 10.0f;

  Serial.print("temp ");
  Serial.println(tempC, 2);
}