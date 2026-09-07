#ifndef OPCODE_H
#define OPCODE_H

#include "common.h"
#include <stdint.h>

uint16_t fetch(Chip8 *chip);
void decode(Chip8 *chip, uint16_t opcode);

#endif
