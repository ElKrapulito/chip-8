#include "chip-8.h"
#include "opcodes.h"
#include <stdio.h>
#include <stdlib.h>

#define BYTE_SIZE 8

void print_binary_custom(uint8_t n) {
  int bits = sizeof(n) * BYTE_SIZE;

  for (int i = bits - 1; i >= 0; i--) {
    unsigned int bit = (n >> i) & 1;
    printf("%c", bit ? '#' : '.');
  }
}

int main() {
  Chip8 *chip = initializeChip("../test/2-ibm-logo.ch8");

  uint16_t op;
  for (int i = 0;i < 20; i++) {
    op = fetch(chip);
    decode(chip, op);
    if (((op >> 12) & 0xF) == 0xD) {
      for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
          // printf("%08b", chip->displaybuffer[j][i]);
          print_binary_custom(chip->displaybuffer[j][i]);
        }
        printf("\n");
      }
      for (int i = 0; i < 16; i++) {
        for (int j = 8; j < 16; j++) {
          // printf("%08b", chip->displaybuffer[j][i]);
          print_binary_custom(chip->displaybuffer[j][i]);
        }
        printf("\n");
      }
    }
  }

  free(chip);
  chip = NULL;
  return 0;
}
