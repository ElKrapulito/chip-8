#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>

typedef struct Config {
    bool bnnn; 
    bool vf_reset;
    bool memory_quirk;
    bool shifting;
    bool noclip;
} Config;

Config* superchip();
Config* chip8();
#endif
