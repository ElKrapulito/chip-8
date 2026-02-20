#include "opcodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void print_binary_custom(uint8_t n) {
    // Get number of bits in an integer
    int bits = sizeof(n) * CHAR_BIT;
    
    for (int i = bits - 1; i >= 0; i--) {
        unsigned int bit = (n >> i) & 1;
        printf("%c", bit ? '#' : '.');
    }
}

int main() {
  Chip8 *chip = initializeChip("../test/1-chip8-logo.ch8");

  uint16_t op;
  for (int i = 0; i < 39; i++) {
    op = fetch(chip);
    decode(chip, op);
  }

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 8; j++) {
      //printf("%08b", chip->displaybuffer[j][i]);
      print_binary_custom(chip->displaybuffer[j][i]);
    }
    printf("\n");
  }
  for (int i = 0; i < 15; i++) {
    for (int j = 8; j < 16; j++) {
      //printf("%08b", chip->displaybuffer[j][i]);
      print_binary_custom(chip->displaybuffer[j][i]);
    }
    printf("\n");
  }

  free(chip);
  chip = NULL;
  return 0;
}
