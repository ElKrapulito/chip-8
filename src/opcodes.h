#include "chip-8.h"
#include <stdint.h>

uint16_t fetch(Chip8 *chip);
void decode(Chip8 *chip, uint16_t opcode);
