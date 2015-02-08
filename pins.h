
#include <Arduino.h>

// Dials for choosing a colour
#define RED_IN_PIN    A2
#define GREEN_IN_PIN  A1
#define BLUE_IN_PIN   A0

// RGB dials have a dual purpose
#define WAGGLE_PIN    A2
#define TWINKLE_PIN   A1
#define SPEED_PIN     A0

// Buttons
#define EDIT_PIN      2
#define ADD_PIN       4
#define DELETE_PIN    5

#define MODE_PIN      7
#define SEQUENCE_PIN  8
#define EASE_PIN      12

// Buzzer
#define BUZZER_PIN    3

// Infra Red Sensor
#define REMOTE_PIN    11

// Main LED Output
#define RED_OUT_PIN   9
#define GREEN_OUT_PIN 10
#define BLUE_OUT_PIN  6

// Indicator light, used during testing, and may be using in the final product.
#define EDIT_LED_PIN  13

// OLED display using I2C interface - NB, these are HARDCODED to these pins, and cannot be changed
#define I2C_SCL       A5
#define I2c_SDA       A4

