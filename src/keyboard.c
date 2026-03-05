#include "keyboard.h"
#include <stdint.h>

#define KEY_1 30
#define KEY_2 31
#define KEY_3 32
#define KEY_4 33

#define KEY_Q 20
#define KEY_W 26
#define KEY_E 8
#define KEY_R 21

#define KEY_A 4
#define KEY_S 22
#define KEY_D 7
#define KEY_F 9

#define KEY_Z 29
#define KEY_X 27
#define KEY_C 6
#define KEY_V 25

uint8_t mapkeycode(uint8_t keycode) {
  switch (keycode) {
  case KEY_X:
    return 0;
  case KEY_1:
    return 1;
  case KEY_2:
    return 2;
  case KEY_3:
    return 3;
  case KEY_Q:
    return 4;
  case KEY_W:
    return 5;
  case KEY_E:
    return 6;
  case KEY_A:
    return 7;
  case KEY_S:
    return 8;
  case KEY_D:
    return 9;
  case KEY_Z:
    return 0xa;
  case KEY_C:
    return 0xb;
  case KEY_4:
    return 0xc;
  case KEY_R:
    return 0xd;
  case KEY_F:
    return 0xe;
  case KEY_V:
    return 0xf;
  default:
    return UINT8_MAX;
  }
}

void keydown(Chip8 *chip, uint8_t keycode) {
  if (keycode <= 0xf) {
    chip->keyboard[keycode] = keycode;
    chip->keycurrent = keycode;
  }
}

void keyup(Chip8 *chip, uint8_t keycode) {
  if (keycode <= 0xf) {
    chip->keyboard[keycode] = UINT8_MAX;
    chip->keycurrent = UINT8_MAX;
  }
}

bool iskeypress(Chip8 *chip, uint8_t keycode) {
  return chip->keyboard[keycode] == keycode;
}
