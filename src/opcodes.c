#include "opcodes.h"
#include "common.h"
#include "keyboard.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint16_t fetch(Chip8 *chip) {
  uint16_t *firstbyte = (uint16_t *)&chip->memory[chip->pc];
  uint16_t *secondbyte = (uint16_t *)&chip->memory[chip->pc + 1];
  uint16_t opcode = (*firstbyte << 8) & 0xFF00 | *secondbyte & 0xFF;
  chip->pc += 2;
  return opcode;
}

void handlezero(Chip8 *chip, uint16_t opcode) {
  switch (opcode) {
  case 0x00e0:
    for (int i = 0; i < 0; i++) {
      for (int j = 0; j < 0; j++) {
        chip->displaybuffer[i][j] = 0;
      }
    }
    printf("[%04x] clear display!\n", opcode);
    break;
  case 0x00ee:
    printf("[%04x] current stack count: %d\n", opcode, chip->stcount);
    chip->stcount -= 1;
    printf("[%04x] new stack count: %d\n", opcode, chip->stcount);
    chip->pc = chip->stack[chip->stcount];
    printf("[%04x] pop stack location: %04x\n", opcode,
           chip->stack[chip->stcount]);
    chip->stack[chip->stcount] = 0;
    break;
  }
}

void handleone(Chip8 *chip, uint16_t opcode) { chip->pc = opcode & 0xFFF; }

void handletwo(Chip8 *chip, uint16_t opcode) {
  uint16_t location = opcode & 0xFFF;
  chip->stack[chip->stcount] = chip->pc;
  chip->pc = location;
  chip->stcount += 1;
}

void handlethree(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t value = opcode & 0xFF;

  if (chip->v[reg] == value) {
    chip->pc += 2;
  }
}

void handlefour(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t value = opcode & 0xFF;

  if (chip->v[reg] != value) {
    chip->pc += 2;
  }
}

void handlefive(Chip8 *chip, uint16_t opcode) {
  uint8_t reg1 = (opcode >> 8) & 0xF;
  uint8_t reg2 = (opcode >> 4) & 0xF;

  if (chip->v[reg1] == chip->v[reg2]) {
    chip->pc += 2;
  }
}

void handlesix(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t value = opcode & 0xFF;
  chip->v[reg] = value;
  printf("[%0x] set register: V%d = %d\n", opcode, reg, value);
}

void handleseven(Chip8 *chip, uint16_t opcode) {
  uint8_t reg = (opcode >> 8) & 0xF;
  uint8_t val = opcode & 0xFF;
  printf("[%04x] reg V%d: %d + %d\n", opcode, reg, chip->v[reg], val);
  chip->v[reg] += val;
  printf("[%04x] reg V%d: %d\n", opcode, reg, chip->v[reg]);
}

void handleeight(Chip8 *chip, uint16_t opcode) {
  uint8_t reg1 = (opcode >> 8) & 0xF;
  uint8_t reg2 = (opcode >> 4) & 0xF;
  uint16_t result;
  switch (opcode & 0xF00F) {
  case 0x8000:
    chip->v[reg1] = chip->v[reg2];
    break;
  case 0x8001:
    chip->v[reg1] |= chip->v[reg2];
    break;
  case 0x8002:
    chip->v[reg1] &= chip->v[reg2];
    break;
  case 0x8003:
    chip->v[reg1] ^= chip->v[reg2];
    break;
  case 0x8004:
    result = chip->v[reg1] + chip->v[reg2];
    chip->v[reg1] = result;
    chip->v[0xF] = result >> 8;
    break;
  case 0x8005:
    result = chip->v[reg1] - chip->v[reg2];
    chip->v[reg1] = result;
    chip->v[0xF] = result > 255 ? 0 : 1;
    break;
  case 0x8006:
    chip->v[0xF] = chip->v[reg1] & 1;
    chip->v[reg1] >>= 1;
    break;
  case 0x8007:
    result = chip->v[reg2] - chip->v[reg1];
    chip->v[reg1] = result;
    chip->v[0xF] = result > 255 ? 0 : 1;
    break;
  case 0x800e:
    chip->v[0xF] = (chip->v[reg1] >> 7) & 1;
    chip->v[reg1] <<= 1;
    break;
  }
}

void handlenine(Chip8 *chip, uint16_t opcode) {
  uint8_t reg1 = (opcode >> 8) & 0xF;
  uint8_t reg2 = (opcode >> 4) & 0xF;

  if (chip->v[reg1] != chip->v[reg2]) {
    chip->pc += 2;
  }
}

void handleA(Chip8 *chip, uint16_t opcode) {
  uint16_t value = opcode & 0xFFF;
  chip->i = value;
  printf("[%04x] set index = %04x\n", opcode, value);
}

void handleB(Chip8 *chip, uint16_t opcode) {
  chip->pc = chip->v[0] + (opcode & 0xFFF);
}

void handleC(Chip8 *chip, uint16_t opcode) {
  int value = opcode & 0xFF;
  int reg = (opcode >> 8) & 0xF;
  int random = (rand() + 1u) % value;
  chip->v[reg] = random;
}

void handleD(Chip8 *chip, uint16_t opcode) {
  uint8_t reg1 = (opcode >> 8) & 0xF;
  uint8_t reg2 = (opcode >> 4) & 0xF;
  uint8_t regVX = chip->v[reg1];
  uint8_t regVY = chip->v[reg2];
  printf("[%04x] VX: %d and VY: %d\n", opcode, regVX, regVY);
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
      chip->displaybuffer[column + 1][row] ^= newpix;
      if (chip->displaybuffer[column + 1][row] >> (8 - colshift) !=
          newpix >> (8 - colshift)) {
        chip->v[0xF] = 1;
      }
    }
    if (chip->displaybuffer[column][row] << colshift != pixels << colshift) {
      chip->v[0xF] = 1;
    }
  }
}

void handleE(Chip8 *chip, uint16_t opcode) {
  uint8_t vx = chip->v[opcode >> 8 & 0xF];
  switch (opcode & 0xF0FF) {
  case 0xe09e:
    if (iskeypress(chip, vx)) {
      chip->pc += 2;
    }
    break;
  case 0xe0a1:
    if (!iskeypress(chip, vx)) {
      chip->pc += 2;
    }
    break;
  }
}

void handleF(Chip8 *chip, uint16_t opcode) {
  uint8_t value = (opcode >> 8) & 0xF;
  uint16_t val;
  uint16_t hun;
  uint16_t ten;
  uint16_t unit;

  switch (opcode & 0xF0FF) {
  case 0xf065:
    for (int i = 0; i <= value; i++) {
      chip->v[i] = chip->memory[chip->i + i];
    }
    break;
  case 0xf055:
    for (int i = 0; i <= value; i++) {
      chip->memory[chip->i + i] = chip->v[i];
    }
    break;
  case 0xf033:
    val = chip->v[value];
    hun = val / 100;
    ten = (val % 100) / 10;
    unit = (val % 100) % 10;

    chip->memory[chip->i] = hun;
    chip->memory[chip->i + 1] = ten;
    chip->memory[chip->i + 2] = unit;
    break;
  case 0xf01e:
    chip->i += chip->v[value];
    if (chip->i > 0xFFF) {
      chip->v[0xf] = 1;
    }
    break;
  default:
    printf("[%04x]*** operation not handle ***\n", opcode);
    return;
  }
}

void decode(Chip8 *chip, uint16_t opcode) {
  uint16_t firstnibble = (opcode & 0xF000) >> 12;

  switch (firstnibble) {
  case 0:
    handlezero(chip, opcode);
    break;
  case 1:
    handleone(chip, opcode);
    break;
  case 2:
    handletwo(chip, opcode);
    break;
  case 3:
    handlethree(chip, opcode);
    break;
  case 4:
    handlefour(chip, opcode);
    break;
  case 5:
    handlefive(chip, opcode);
    break;
  case 6:
    handlesix(chip, opcode);
    break;
  case 7:
    handleseven(chip, opcode);
    break;
  case 8:
    handleeight(chip, opcode);
    break;
  case 9:
    handlenine(chip, opcode);
    break;
  case 0xa:
    handleA(chip, opcode);
    break;
  case 0xb:
    handleB(chip, opcode);
    break;
  case 0xc:
    handleC(chip, opcode);
    break;
  case 0xe:
    handleE(chip, opcode);
    break;
  case 0xd:
    handleD(chip, opcode);
    break;
  case 0xf:
    handleF(chip, opcode);
    break;
  default:
    printf("[%04x]*** operation not handle ***\n", opcode);
    return;
  }
}
