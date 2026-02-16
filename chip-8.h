#include <stdint.h>

typedef struct Chip8 {
	uint16_t pc;
	uint16_t i;
	uint16_t memory[2048];
	uint16_t *stack;
	uint8_t delay;
	uint8_t v[16];
	uint8_t sound_t;
} Chip8;
