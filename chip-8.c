#include "chip-8.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t *fonts[16];
uint8_t zero[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, '\0'};
uint8_t one[] = {0x20, 0x60, 0x20, 0x20, 0x70, '\0'};
uint8_t two[] = {0xF0, 0x10, 0xF0, 0x80, 0xF0, '\0'};
uint8_t three[] = {0xF0, 0x10, 0xF0, 0x10, 0xF0, '\0'};
uint8_t four[] = {0x90, 0x90, 0xF0, 0x10, 0x10, '\0'};
uint8_t five[] = {0xF0, 0x80, 0xF0, 0x10, 0xF0, '\0'};
uint8_t six[] = {0xF0, 0x80, 0xF0, 0x90, 0xF0, '\0'};
uint8_t seven[] = {0xF0, 0x10, 0x20, 0x40, 0x40, '\0'};
uint8_t eight[] = {0xF0, 0x90, 0xF0, 0x90, 0xF0, '\0'};
uint8_t nine[] = {0xF0, 0x90, 0xF0, 0x10, 0xF0, '\0'};
uint8_t letter_a[] = {0xF0, 0x90, 0xF0, 0x90, 0x90, '\0'};
uint8_t letter_b[] = {0xE0, 0x90, 0xE0, 0x90, 0xE0, '\0'};
uint8_t letter_c[] = {0xF0, 0x80, 0x80, 0x80, 0xF0, '\0'};
uint8_t letter_d[] = {0xE0, 0x90, 0x90, 0x90, 0xE0, '\0'};
uint8_t letter_e[] = {0xF0, 0x80, 0xF0, 0x80, 0xF0, '\0'};
uint8_t letter_f[] = {0xF0, 0x80, 0xF0, 0x80, 0x80, '\0'};

int loadfonts(Chip8 *chip) {
  fonts[0] = zero;
  fonts[1] = one;
  fonts[2] = two;
  fonts[3] = three;
  fonts[4] = four;
  fonts[5] = five;
  fonts[6] = six;
  fonts[7] = seven;
  fonts[8] = eight;
  fonts[9] = nine;
  fonts[0xA] = letter_a;
  fonts[0xB] = letter_b;
  fonts[0xC] = letter_c;
  fonts[0xD] = letter_d;
  fonts[0xE] = letter_e;
  fonts[0xF] = letter_f;
  uint8_t *memlocation = &chip->memory[0x50];

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%d : loading fonts font: %02x and value: %02x\n", i, fonts[i],
             *fonts[i]);
      *memlocation = *fonts[i];
      memlocation++;
      fonts[i]++;
    }
  }

  memlocation = &chip->memory[0x50];
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      printf("%d : values loaded in memory %02x\n", i, *memlocation);
      *memlocation = *fonts[i];
      memlocation++;
      fonts[i]++;
    }
  }
  return 0;
}

Chip8 *createChip() {
  Chip8 *chip = malloc(sizeof(Chip8));
  return chip;
}
