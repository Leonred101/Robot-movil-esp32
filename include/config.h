#pragma once
#include <Arduino.h>

// IR sensor and light sensor (analog and digital)
constexpr int PIN_IR = 32;      // ADC1_CH4
constexpr int PIN_PHOTORA = 34; // ADC1_CH6
constexpr int PIN_PHOTORD = 19; // Digital input for digital light reading with pull-up
constexpr int PIN_CONTACT = 18; // Digital input for contact with pull-down
constexpr int PIN_TEMP = 35;    // ADC1_CH7, used for LM35 sensor

// ===== Unused sensors for now =====
// constexpr int PIN_TEMP    = 35; // ADC1_CH7
