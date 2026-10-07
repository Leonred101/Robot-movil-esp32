#pragma once
#include <Arduino.h>
#include "sensor_manager.h"
#include "motors.h"

inline void printGlobalMenu() {
  Serial.println("\n==================================================");
  Serial.println("       ROBOT MOVIL - INTERFAZ DE COMANDOS         ");
  Serial.println("==================================================");
  Serial.println("Ayuda:");
  Serial.println("  h                 -> Muestra este menu");
  Serial.println("\nSensores:");
  printSensorsHelp();
  Serial.println("\nActuadores (Practica 3):");
  Serial.println("  A1/A2 on/off left/right -> Control discreto");
  Serial.println("  A1/A2 speed <-127..127> -> Control bipolar PWM");
  Serial.println("==================================================\n");
}

inline void parseCommandLine(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  if (cmd.equalsIgnoreCase("h") || cmd.equalsIgnoreCase("help")) {
    printGlobalMenu();
    return;
  }

  // 1. Intenta evaluar como comando de sensores
  if (processSensorCommand(cmd)) {
    return;
  }

  // 2. Intenta evaluar como comando de motores (Practica 3)
  if (processMotorCommand(cmd)) {
    return;
  }

  Serial.println("Comando no valido. Teclea 'h' para ver el menu.");
}