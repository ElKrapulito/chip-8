#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#define SDL_MAIN_USE_CALLBACKS 1
#define BYTE_SIZE 8

#include "chip-8.h"
#include "common.h"
#include "keyboard.h"
#include "opcodes.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Chip8 *chip = NULL;
static Uint64 last_time = 0;

#define WINDOWS_WIDTH 64
#define WINDOWS_HEIGHT 32
#define SCALE_PX 5

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_SetAppMetadata("Chip8", "1.0", "com.chip8");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't start window: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("chip8", WINDOWS_WIDTH * SCALE_PX,
                                   WINDOWS_HEIGHT * SCALE_PX,
                                   SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("couldn't create window and renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetRenderLogicalPresentation(renderer, WINDOWS_WIDTH * SCALE_PX,
                                   WINDOWS_HEIGHT * SCALE_PX,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  last_time = SDL_GetTicks();

  printf("rom route: %s\n", argv[1]);
  chip = initializeChip(argv[1]);

  if (chip == NULL) {
    SDL_Log("failed to initialize chip: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  uint8_t chipkey;
  switch (event->type) {
  case SDL_EVENT_KEY_DOWN:
    chipkey = mapkeycode(event->key.scancode);
    printf("[EVENT] key pressed: %d chip key: %d\n", event->key.scancode,
           chipkey);
    keydown(chip, event->key.scancode);
    break;
  case SDL_EVENT_KEY_UP:
    chipkey = mapkeycode(event->key.scancode);
    printf("[EVENT] key released: %d chip key: %d\n", event->key.scancode,
           chipkey);
    keyup(chip, event->key.scancode);
    break;
  case SDL_EVENT_QUIT:
    free(chip);
    chip = NULL;
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

uint16_t opcode;
SDL_FRect rects[2048];

SDL_AppResult SDL_AppIterate(void *appstate) {
  // const Uint64 now = SDL_GetTicks();
  // const float elapsed = ((float)(now - last_time)) / 1000.0f;

  opcode = fetch(chip);
  decode(chip, opcode);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  if (((opcode >> 12) & 0xF) == 0xD) {
    int recti = 0;
    for (int i = 0; i < 16; i++) {
      for (int j = 0; j < 16; j++) {
        int x = (i & 7) * 8;
        int y = (i >= 8 ? 16 : 0) + j;
        uint8_t pixels = chip->displaybuffer[i][j];
        for (int p = 7; p >= 0; p--) {
          uint8_t bit = (pixels >> p) & 1;
          if (bit == 1) {
            int col = x + (8 - (p + 1));
            rects[recti].x = col * SCALE_PX;
            rects[recti].y = y * SCALE_PX;
            rects[recti].w = rects[recti].h = 1 * SCALE_PX;
            recti++;
          }
        }
      }
    }
  }

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRects(renderer, rects, SDL_arraysize(rects));

  SDL_RenderPresent(renderer);
  // last_time = now;

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  free(chip);
  chip = NULL;
}
