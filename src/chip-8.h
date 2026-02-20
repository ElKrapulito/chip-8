#include <stdint.h>
#include <stdio.h>

typedef struct Chip8 {
	uint16_t pc;
	uint16_t i;
	uint16_t *stack;
	uint8_t memory[4096];
	uint8_t delay;
	uint8_t v[16];
	uint8_t sound_t;
	uint8_t displaybuffer [16][16];
} Chip8;

void loadfonts(Chip8 *chip);
void loadfile(Chip8 *chip, char *path);
Chip8 *createChip();
Chip8 *initializeChip(char *filepath);
