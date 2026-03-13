#include "config.h"
#include <stdlib.h>

Config *superchip() {
  Config *config = malloc(sizeof(Config));
  config->bnnn = true;
  config->memory_quirk = false;
  config->shifting = false;
  config->vf_reset = false;
  config->noclip = false;
  return config;
}

Config *chip8() {
  Config *config = malloc(sizeof(Config));
  config->bnnn = false;
  config->memory_quirk = true;
  config->shifting = true;
  config->vf_reset = true;
  config->noclip = false;
  return config;
}
