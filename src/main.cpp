#include "config.h"
#include "sensor_contact.h"
#include "sensor_temp.h"
#include "sensor_light.h"
#include "sensor_ir.h"

// Function type for sensor reading
typedef void (*SensorCallback)();

// Structure to associate command, function, and menu description
struct SensorCommand {
  const char* name;
  SensorCallback execute;
  const char* description;
};

// Available sensors table
const SensorCommand SENSOR_TABLE[] = {
  {"contact",  readContact, "Digital contact sensor"},
  {"photora",  readPhotoRA, "Photoresistor (Analog reading)"},
  {"photord",  readPhotoRD, "Photoresistor (LM339 digital output)"},
  {"temp",     readTemp,    "LM35 temperature sensor"},
  {"infrared", readIR,      "Sharp infrared distance sensor"}
};

constexpr size_t NUM_SENSORS = sizeof(SENSOR_TABLE) / sizeof(SENSOR_TABLE[0]);

bool firstRun = true; // Control flag for the first iteration

void printMenu() {
  Serial.println();
  Serial.println("==================================================");
  Serial.println("          ESP32 SENSOR SYSTEM - MENU              ");
  Serial.println("==================================================");
  Serial.println("Available commands:");
  Serial.println("  h                 -> Show this help menu");
  for (size_t i = 0; i < NUM_SENSORS; ++i) {
    Serial.print("  shs ");
    Serial.print(SENSOR_TABLE[i].name);
    Serial.print("\t-> Single reading of ");
    Serial.println(SENSOR_TABLE[i].description);
  }
  Serial.println("==================================================");
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  // Full scale for analog readings (0 to ~3.1 V)
  analogSetAttenuation(ADC_11db);

  // Hardware initialization
  initContact();
  initLight();
  initTemp();
  initIR();
}

void loop() {
  // Automatic display only on the first iteration
  if (firstRun) {
    printMenu();
    firstRun = false;
  }

  // On-demand Serial command processing
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() == 0) {
      return; // Discard empty Enter keystrokes
    }

    // Display menu when pressing 'h' or 'H'
    if (input.equalsIgnoreCase("h") || input.equalsIgnoreCase("help")) {
      printMenu();
    }
    // Execute single measurement for the requested sensor
    else if (input.startsWith("shs ")) {
      String sensorReq = input.substring(4);
      sensorReq.trim();

      bool commandFound = false;

      for (size_t i = 0; i < NUM_SENSORS; ++i) {
        if (sensorReq.equalsIgnoreCase(SENSOR_TABLE[i].name)) {
          // Trigger the function once
          SENSOR_TABLE[i].execute();
          commandFound = true;
          break;
        }
      }

      if (!commandFound) {
        Serial.println("Error: Sensor not recognized. Type 'h' to see the command list.");
      }
    }
    else {
      Serial.println("Invalid command. Type 'h' to check the menu.");
    }
  }
}
