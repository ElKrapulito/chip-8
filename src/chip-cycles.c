#include "chip-cycles.h"
#include "opcodes.h"
#include <stdio.h>

uint16_t opcode;
uint32_t cpucount;

void cycle(Chip8 *chip, uint32_t amountOfOperations) {
  uint32_t i;
  for (i = 0; i <= amountOfOperations; i++) {
    opcode = fetch(chip);
    decode(chip, opcode);
  }
}

void stop() { 
//    stoploop = true; 
}
