#ifndef COMMON_H
#define COMMON_H
#include <stdint.h>

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
#endif
