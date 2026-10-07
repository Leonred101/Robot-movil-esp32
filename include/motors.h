#pragma once
#include <Arduino.h>
#include "config.h"

// Canales y parametros PWM para ESP32
constexpr int PWM_FREQ     = 1000; // 1 kHz estándar para L293D
constexpr int PWM_RES_BITS = 8;    // 8 bits de resolucion (0 a 255)
constexpr int PWM_CH_M1    = 0;    // Canal LEDC 0
constexpr int PWM_CH_M2    = 1;    // Canal LEDC 1

// Modos de operacion de la Practica 3
enum MotorControlMode {
  MODE_DISCRETE, // Punto 2: Control directo digital (on/off, left/right)
  MODE_BIPOLAR   // Punto 3: Control continuo bipolar por PWM
};

inline MotorControlMode currentMotorMode = MODE_DISCRETE;

// Inicializacion de pines y canales PWM
inline void initMotors() {
  pinMode(PIN_M1_PWM_OR_IN1, OUTPUT);
  pinMode(PIN_M1_IN2, OUTPUT);
  pinMode(PIN_M2_PWM_OR_IN1, OUTPUT);
  pinMode(PIN_M2_IN2, OUTPUT);

  // Configuracion de canales PWM para ESP32 (LEDC)
  ledcSetup(PWM_CH_M1, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(PIN_M1_PWM_OR_IN1, PWM_CH_M1);

  ledcSetup(PWM_CH_M2, PWM_FREQ, PWM_RES_BITS);
  ledcAttachPin(PIN_M2_PWM_OR_IN1, PWM_CH_M2);

  // Iniciar motores apagados (0V en digital / 50% en bipolar)
  digitalWrite(PIN_M1_PWM_OR_IN1, LOW);
  digitalWrite(PIN_M1_IN2, LOW);
  digitalWrite(PIN_M2_PWM_OR_IN1, LOW);
  digitalWrite(PIN_M2_IN2, LOW);
}

// ----------------------------------------------------------------------
// METODO PUNTO 2: Control digital directo (on/off, left/right)
// ----------------------------------------------------------------------
inline void setMotorDiscrete(int motorNum, bool turnOn, bool turnLeft) {
  currentMotorMode = MODE_DISCRETE;
  
  // Desvincula temporalmente el PWM si estaba activo para control digital puro
  int pinA = (motorNum == 1) ? PIN_M1_PWM_OR_IN1 : PIN_M2_PWM_OR_IN1;
  int pinB = (motorNum == 1) ? PIN_M1_IN2 : PIN_M2_IN2;
  int ch   = (motorNum == 1) ? PWM_CH_M1 : PWM_CH_M2;

  ledcDetachPin(pinA);
  pinMode(pinA, OUTPUT);

  if (!turnOn) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    Serial.printf("Motor A%d detenido (OFF)\n", motorNum);
    return;
  }

  if (turnLeft) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    Serial.printf("Motor A%d ON -> LEFT\n", motorNum);
  } else {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    Serial.printf("Motor A%d ON -> RIGHT\n", motorNum);
  }
}

// ----------------------------------------------------------------------
// METODO PUNTO 3: Control Bipolar PWM con Inversor SN74LS14
// Valor recibido: -127 a +127
// - 0      -> 50% Duty Cycle (127/255) -> Motor Detenido
// - < 0    -> Duty < 50%               -> Giro a la DERECHA
// - > 0    -> Duty > 50%               -> Giro a la IZQUIERDA
// ----------------------------------------------------------------------
inline void setMotorSpeedBipolar(int motorNum, int speedVal) {
  currentMotorMode = MODE_BIPOLAR;

  // Limitar al rango estricto [-127, 127]
  if (speedVal < -127) speedVal = -127;
  if (speedVal > 127)  speedVal = 127;

  // Reconectar al canal PWM si se habia desacoplado
  int pinA = (motorNum == 1) ? PIN_M1_PWM_OR_IN1 : PIN_M2_PWM_OR_IN1;
  int ch   = (motorNum == 1) ? PWM_CH_M1 : PWM_CH_M2;
  ledcAttachPin(pinA, ch);

  // Mapeo: speedVal (-127 a 127) -> duty (0 a 254/255)
  // -127 => duty 0
  //    0 => duty 127 (50%)
  // +127 => duty 254 (~100%)
  int duty = speedVal + 127;

  ledcWrite(ch, duty);

  Serial.printf("Motor A%d speed: %d (Duty Cycle: %d/255 -> %.1f%%)\n", 
                motorNum, speedVal, duty, (duty / 255.0f) * 100.0f);
}

// ----------------------------------------------------------------------
// PARSER DE MOTORES: Procesa lineas que comiencen con A1 o A2
// ----------------------------------------------------------------------
inline bool processMotorCommand(const String& cmd) {
  if (!cmd.startsWith("A1") && !cmd.startsWith("A2") &&
      !cmd.startsWith("a1") && !cmd.startsWith("a2")) {
    return false; // No pertenece al subsistema de motores
  }

  int motorNum = (cmd.charAt(1) == '1') ? 1 : 2;

  // Tokens esperados:
  // 1) A1 on left | A1 on right | A1 off
  // 2) A1 speed <valor>
  char motorStr[5], action[10], param[10];
  int count = sscanf(cmd.c_str(), "%s %s %s", motorStr, action, param);

  if (count < 2) {
    Serial.println("Error sintaxis: use 'A1 on left', 'A1 off' o 'A1 speed <val>'");
    return true;
  }

  String actStr = String(action);
  actStr.toLowerCase();

  // Caso 1: Punto 3 -> speed <valor>
  if (actStr == "speed") {
    if (count < 3) {
      Serial.println("Error: Falta el valor numerico de velocidad (-127 a +127)");
      return true;
    }
    int speedVal = atoi(param);
    setMotorSpeedBipolar(motorNum, speedVal);
    return true;
  }

  // Caso 2: Punto 2 -> off
  if (actStr == "off") {
    setMotorDiscrete(motorNum, false, false);
    return true;
  }

  // Caso 3: Punto 2 -> on left / on right
  if (actStr == "on") {
    if (count < 3) {
      Serial.println("Error: Especifique direccion ('left' o 'right')");
      return true;
    }
    String dirStr = String(param);
    dirStr.toLowerCase();
    if (dirStr == "left") {
      setMotorDiscrete(motorNum, true, true);
    } else if (dirStr == "right") {
      setMotorDiscrete(motorNum, true, false);
    } else {
      Serial.println("Error: Direccion invalida. Use 'left' o 'right'");
    }
    return true;
  }

  Serial.println("Error: Accion de motor no reconocida.");
  return true;
}