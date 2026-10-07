#pragma once
#include "config.h"

// Initialization of the analog and digital parts of the light sensor.
inline void initLight() {
  pinMode(PIN_PHOTORA, INPUT); // Analog input
  pinMode(PIN_PHOTORD, INPUT_PULLUP); // Digital input with pull-up
}

// Analog reading of the PhotoRA light sensor.
inline void readPhotoRA() {
  int raw = analogRead(PIN_PHOTORA);
  //Serial.print("photora ");
  Serial.println(raw);
}

// Digital reading of the PhotoRD light sensor.
inline void readPhotoRD() {
  int state = digitalRead(PIN_PHOTORD);
  //Serial.print("photord ");
  Serial.println(state);
}
