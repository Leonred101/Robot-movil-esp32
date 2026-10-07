#include "config.h"
#include "sensor_contact.h"
#include "sensor_temp.h"
#include "sensor_light.h"
#include "sensor_ir.h"

// Definimos un tipo de función que representa la lectura de un sensor.
// Cada sensor tendrá una función que se ejecuta cuando está activo.
typedef void (*SensorCallback)();

// Estructura para asociar un nombre de comando con la función del sensor.
struct SensorCommand {
  const char* name;     // Nombre que llega por Serial, por ejemplo: "infrared"
  SensorCallback execute; // Función que toma la medición del sensor
};

// Tabla de sensores disponibles.
// Por ahora solo queda activado el sensor IR; el resto está comentado.
const SensorCommand SENSOR_TABLE[] = {
  {"contact",  readContact},
  {"photora",  readPhotoRA},
  {"photord",  readPhotoRD},
  {"temp",     readTemp},
  {"infrared", readIR}
};

// Número de elementos en la tabla de sensores.
constexpr size_t NUM_SENSORS = sizeof(SENSOR_TABLE) / sizeof(SENSOR_TABLE[0]);

// Puntero al sensor actualmente activo.
// Mientras este apuntador no sea nulo, el sistema seguirá haciendo lecturas.
SensorCallback activeSensorCallback = nullptr;

// Frecuencia moderada para que la lectura sea visible y legible.
// Ajusta el valor de 200 ms si quieres hacerlo más rápido o más lento.
const uint32_t SENSOR_DELAY_MS = 200;

void setup() {
  // Inicia la comunicación serie para recibir comandos y enviar mediciones.
  Serial.begin(115200);

  // Ajusta la escala del ADC para leer mejor la señal analógica del IR.
  analogSetAttenuation(ADC_11db);

  // Inicializa los sensores que sí se usarán en esta etapa.
  initContact();
  initLight();
  initTemp();
  initIR();
}

void loop() {
  // Primero revisamos si el usuario envió un comando por Serial.
  // Esto permite cambiar de sensor sin que el sensor activo bloquee la entrada.
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    // Si el usuario pulsa Enter sin texto, se detiene la lectura actual.
    if (input.length() == 0) {
      activeSensorCallback = nullptr;
      Serial.println("Lectura detenida");
    }
    // Comando para activar un sensor.
    else if (input.startsWith("shs ")) {
      String sensorReq = input.substring(4);
      sensorReq.trim();

      bool commandFound = false;

      // Busca el nombre del sensor dentro de la tabla de comandos.
      for (size_t i = 0; i < NUM_SENSORS; ++i) {
        if (sensorReq.equalsIgnoreCase(SENSOR_TABLE[i].name)) {
          // Se activa el sensor solicitado para que siga leyendo con el retraso definido.
          activeSensorCallback = SENSOR_TABLE[i].execute;
          commandFound = true;
          break;
        }
      }

      // Si no existe el comando, se deja sin sensor activo.
      if (!commandFound) {
        Serial.println("Error: Sensor no reconocido");
        activeSensorCallback = nullptr;
      }
    }
  }

  // La lectura del sensor se repite cada 200 ms para que sea visible y cómoda.
  // Solo se detiene cuando el usuario presiona Enter vacío.
  if (activeSensorCallback != nullptr) {
    activeSensorCallback();
    delay(SENSOR_DELAY_MS);
  }
}
 