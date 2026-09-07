#ifndef CHIP_CYCLES_H
#define CHIP_CYCLES_H

#include "common.h"
#include <stdint.h>

void cycle(Chip8 *chip, uint32_t amountOfOperations);
void stop();

#endif
