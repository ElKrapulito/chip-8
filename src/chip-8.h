#include <stdint.h>
#include <stdio.h>
#include "common.h"

void loadfonts(Chip8 *chip);
int loadfile(Chip8 *chip, char *path);
Chip8 *createChip();
Chip8 *initializeChip(char *filepath);
