#include <Arduino.h>
#include <Wire.h>
#include "./UIService.hpp"

extern const uint8_t LCD_SDA;
extern const uint8_t LCD_SCL;
extern const uint8_t LCD_DC;
extern const uint8_t LCD_CS[6];
extern const uint8_t BTN[6];
extern uint8_t LCD_OPA;

static bool game_started = false;

UIService UI;