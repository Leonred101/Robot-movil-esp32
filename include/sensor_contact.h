#pragma once
#include "config.h"

inline void initContact() {
  // The contact is configured with pull-down to remain in low state
  // by default and change to HIGH when the circuit is closed.
  pinMode(PIN_CONTACT, INPUT_PULLDOWN);
}

inline void readContact() {
  int state = digitalRead(PIN_CONTACT);
  //Serial.print("contact ");
  Serial.println(state);
}
