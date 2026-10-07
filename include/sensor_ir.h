#pragma once
#include "config.h"

// Este archivo contiene la lógica activa para el sensor IR.
// El resto de sensores queda comentado temporalmente en el proyecto.

inline void initIR() {
  // Pines analógicos quedan como INPUT por defecto.
}

inline void readIR() {
  // Filtro de promedio móvil por sobremuestreo (16 lecturas).
  uint32_t sum = 0;
  for (int i = 0; i < 16; ++i) {
    sum += analogReadMilliVolts(PIN_IR);
    delayMicroseconds(150);
  }
  uint32_t mv = sum / 16;

  Serial.print("infrared ");
  Serial.println(mv); // Entrega la lectura estable en mV.
}