#include "chip-8.h"
#include <stdlib.h>

int main() {
  Chip8 *chip = createChip();
  loadfonts(chip);
  loadfile(chip, "./test/1-chip8-logo.ch8");
  
  free(chip);
  chip = NULL;
  return 0;
}
