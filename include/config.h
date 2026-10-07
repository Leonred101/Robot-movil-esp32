#pragma once
#include <Arduino.h>

// ===== SENSORS (Practica 2) =====
constexpr int PIN_IR      = 32; // ADC1_CH4 (Sharp IR)
constexpr int PIN_PHOTORA = 34; // ADC1_CH6 (LDR Analog)
constexpr int PIN_PHOTORD = 19; // LM339 Digital (Input Pullup)
constexpr int PIN_CONTACT = 18; // Pushbutton (Input Pulldown)
constexpr int PIN_TEMP    = 35; // ADC1_CH7 (LM35)

// ===== ACTUATORS - L293D (Practica 3) =====
// Motor 1 (Izquierdo / A1)
constexpr int PIN_M1_PWM_OR_IN1 = 25; // PWM bipolar (Punto 3) o INPUT1 (Punto 2)
constexpr int PIN_M1_IN2        = 26; // INPUT2 (Punto 2) o no conectado si usas inversor 7414

// Motor 2 (Derecho / A2)
constexpr int PIN_M2_PWM_OR_IN1 = 27; // PWM bipolar (Punto 3) o INPUT3 (Punto 2)
constexpr int PIN_M2_IN2        = 14; // INPUT4 (Punto 2) o no conectado si usas inversor 7414