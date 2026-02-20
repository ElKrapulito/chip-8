#include "opcodes.h"
#include <stdint.h>

uint16_t fetch(Chip8 *chip) {
  uint16_t *firstbyte = (uint16_t *)&chip->memory[chip->pc];
  uint16_t *secondbyte = (uint16_t *)&chip->memory[chip->pc + 1];
  uint16_t opcode = (*firstbyte << 8) & 0xFF00 | *secondbyte & 0xFF;
  chip->pc += 2;
  return opcode;
}

void handlezero(uint16_t opcode) {
  switch (opcode) {
  case 0x00e0:
    printf("clear display!\n");
    break;
  }
}

void handlesix(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t value = opcode & 0xFF;
  chip->v[reg] = value;
  printf("set register: V%d = %d\n", reg, value);
}

void handleA(Chip8 *chip, uint16_t opcode) {
  uint16_t value = opcode & 0xFFF;
  chip->i = value;
  printf("set index = %04x\n", value);
}

void handleD(Chip8 *chip, uint16_t opcode) {
  uint8_t reg1 = (opcode >> 8) & 0xF;
  uint8_t reg2 = (opcode >> 4) & 0xF;
  uint8_t regVX = chip->v[reg1];
  uint8_t regVY = chip->v[reg2];
  printf("VX: %d and VY: %d\n", regVX, regVY);
  uint8_t coordX = regVX & 63;
  uint8_t coordY = regVY & 31;
  chip->v[0xF] = 0;
  uint8_t rows = opcode & 0xF;
  printf("%02x\n", rows);
  uint8_t pixels;
  uint8_t bottom = coordY < 16 ? 0: 8;
  uint8_t xcoord = (coordX >> 3) + bottom;
  for (int i = 0; i < rows; i++) {
	pixels = chip->memory[chip->i + i];
  	printf("%08b\n", pixels);
	chip->displaybuffer[xcoord][i] = pixels;
  }
  printf("xcoord: %d\n", xcoord);
}

void decode(Chip8 *chip, uint16_t opcode) {
  uint16_t firstnibble = (opcode & 0xF000) >> 12;
  printf("opcode: %04x\n", opcode);
  printf("first nibble: %d\n", firstnibble);

  switch (firstnibble) {
  case 0:
    handlezero(opcode);
    break;
  case 6:
    handlesix(chip, opcode);
    break;
  case 10:
    handleA(chip, opcode);
    break;
  case 13:
    handleD(chip, opcode);
    break;
  default:
    printf("operation not handle: %04x\n", opcode);
    return;
  }
}
