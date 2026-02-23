#include "opcodes.h"
#include <stdint.h>
#include <stdio.h>

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

void handleone(Chip8 *chip, uint16_t opcode) {
	chip->pc = opcode & 0xFFF;
	//printf("[%04x] jump to %04x\n", opcode, opcode & 0xFFF);
}

void handlesix(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t value = opcode & 0xFF;
  chip->v[reg] = value;
  printf("set register: V%d = %d\n", reg, value);
}

void handleseven(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t val = opcode & 0xFF;
  printf("[%04x] reg V%d: %d + %d\n",opcode, reg, chip->v[reg], val);
  chip->v[reg] += val;
  printf("[%04x] reg V%d: %d\n", opcode, reg, chip->v[reg]);
}

void handleA(Chip8 *chip, uint16_t opcode) {
  uint16_t value = opcode & 0xFFF;
  chip->i = value;
  printf("[%04x] set index = %04x\n", opcode, value);
}

void handleD(Chip8 *chip, uint16_t opcode) {
  uint8_t reg1 = (opcode >> 8) & 0xF;
  uint8_t reg2 = (opcode >> 4) & 0xF;
  uint8_t regVX = chip->v[reg1];
  uint8_t regVY = chip->v[reg2];
  printf("[%04x] VX: %d and VY: %d\n",opcode, regVX, regVY);
  uint8_t coordX = regVX & 63;
  uint8_t coordY = regVY & 31;
  chip->v[0xF] = 0;
  uint8_t rows = opcode & 0xF;
  printf("[%04x] rows: %02x\n", opcode, rows);
  printf("[%04x] index: %04x\n", opcode, chip->i);
  uint8_t pixels;

  for (int i = 0; i < rows; i++) {
    uint8_t row = (coordY + i) & 15;
    uint8_t bottom = (coordY + i) < 16 ? 0 : 8;
    uint8_t column = (coordX >> 3) + bottom;
    uint8_t colshift = coordX & 7;

    pixels = chip->memory[chip->i + i];
    chip->displaybuffer[column][row] ^= pixels >> colshift;
    if (colshift > 0) {
      uint8_t newpix = pixels << (8 - colshift);
      printf("[%04x] shift: %d\n", opcode, colshift);
      printf("[%04x] inverse shift: %d\n", opcode, 8 - colshift);
      printf("[%04x] og pixels: %08b\n", opcode, pixels);
      printf("[%04x] shift pixels: %08b\n", opcode, pixels >> colshift);
      printf("[%04x] inverse pixels: %08b\n", opcode, newpix);
      chip->displaybuffer[column + 1][row] ^= newpix;
    }
  }
}

void decode(Chip8 *chip, uint16_t opcode) {
  uint16_t firstnibble = (opcode & 0xF000) >> 12;
  // printf("opcode: %04x\n", opcode);
  //printf("first nibble: %d\n", firstnibble);

  switch (firstnibble) {
  case 0:
    handlezero(opcode);
    break;
  case 1:
    handleone(chip, opcode);
    break;
  case 6:
    handlesix(chip, opcode);
    break;
  case 7:
    handleseven(chip, opcode);
    break;
  case 10:
    handleA(chip, opcode);
    break;
  case 13:
    handleD(chip, opcode);
    break;
  default:
    printf("[%04x] operation not handle\n", opcode);
    return;
  }
}
