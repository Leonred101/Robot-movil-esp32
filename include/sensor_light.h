#pragma once
#include "config.h"

// Inicialización de la parte analógica y digital del sensor de luz.
inline void initLight() {
  pinMode(PIN_PHOTORA, INPUT); // Entrada analógica
  pinMode(PIN_PHOTORD, INPUT_PULLUP); // Entrada digital con pull-up
}

// Lectura analógica del sensor de luz PhotoRA.
inline void readPhotoRA() {
  int raw = analogRead(PIN_PHOTORA);
  Serial.print("photora ");
  Serial.println(raw);
}

// Lectura digital del sensor de luz PhotoRD.
inline void readPhotoRD() {
  int state = digitalRead(PIN_PHOTORD);
  Serial.print("photord ");
  Serial.println(state);
}