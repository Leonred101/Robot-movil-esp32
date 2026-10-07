#pragma once
#include <Arduino.h>
#include "config.h"
#include "sensor_contact.h"
#include "sensor_temp.h"
#include "sensor_light.h"
#include "sensor_ir.h"

typedef void (*SensorCallback)();

struct SensorCommand {
  const char* name;
  SensorCallback execute;
  const char* description;
};

const SensorCommand SENSOR_TABLE[] = {
  {"contact",  readContact, "Digital contact sensor"},
  {"photora",  readPhotoRA, "Photoresistor (Analog)"},
  {"photord",  readPhotoRD, "Photoresistor (LM339 digital)"},
  {"temp",     readTemp,    "LM35 temperature sensor"},
  {"infrared", readIR,      "Sharp IR distance sensor (cm)"}
};

constexpr size_t NUM_SENSORS = sizeof(SENSOR_TABLE) / sizeof(SENSOR_TABLE[0]);

inline void initSensors() {
  analogSetAttenuation(ADC_11db);
  initContact();
  initLight();
  initTemp();
  initIR();
}

// Retorna true si el comando fue procesado por este modulo
inline bool processSensorCommand(const String& cmd) {
  if (!cmd.startsWith("shs ")) {
    return false;
  }

  String sensorReq = cmd.substring(4);
  sensorReq.trim();

  for (size_t i = 0; i < NUM_SENSORS; ++i) {
    if (sensorReq.equalsIgnoreCase(SENSOR_TABLE[i].name)) {
      SENSOR_TABLE[i].execute();
      return true;
    }
  }

  Serial.println("Error: Sensor no reconocido.");
  return true;
}

inline void printSensorsHelp() {
  for (size_t i = 0; i < NUM_SENSORS; ++i) {
    Serial.print("  shs ");
    Serial.print(SENSOR_TABLE[i].name);
    Serial.print("\t-> ");
    Serial.println(SENSOR_TABLE[i].description);
  }
}