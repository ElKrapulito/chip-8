#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "common.h"
#include <stdbool.h>

uint8_t mapkeycode(uint8_t keycode);
void keydown(Chip8 *chip, uint8_t keycode);
void keyup(Chip8 *chip, uint8_t keycode);
bool iskeypress(Chip8 *chip, uint8_t keycode);
#endif
