#pragma once
#include <Arduino.h>

// Sensor IR y sensor de luz (analógico y digital)
constexpr int PIN_IR = 32;      // ADC1_CH4
constexpr int PIN_PHOTORA = 34; // ADC1_CH6
constexpr int PIN_PHOTORD = 19; // Entrada digital para lectura de luz digital con pull-up
constexpr int PIN_CONTACT = 18; // Entrada digital para contacto con pull-down
constexpr int PIN_TEMP = 35;    // ADC1_CH7, usado para sensor LM35

// ===== Sensores no usados por ahora =====
// constexpr int PIN_TEMP    = 35; // ADC1_CH7