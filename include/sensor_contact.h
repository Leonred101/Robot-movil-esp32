#pragma once
#include "config.h"

inline void initContact() {
  // El contacto se configura con pull-down para quedar en estado bajo
  // por defecto y cambiar a HIGH cuando se cierre el circuito.
  pinMode(PIN_CONTACT, INPUT_PULLDOWN);
}

inline void readContact() {
  int state = digitalRead(PIN_CONTACT);
  Serial.print("contact ");
  Serial.println(state);
}