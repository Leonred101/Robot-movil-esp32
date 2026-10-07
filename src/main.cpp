#include <Arduino.h>
#include "parser.h"

bool firstRun = true;

void setup() {
  Serial.begin(115200);
  
  initSensors();
  initMotors();
}

void loop() {
  if (firstRun) {
    printGlobalMenu();
    firstRun = false;
  }

  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    parseCommandLine(line);
  }
}