#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_timer.h>
#include <math.h>
#define SDL_MAIN_USE_CALLBACKS 1
#define BYTE_SIZE 8

#include "chip-8.h"
#include "chip-cycles.h"
#include "common.h"
#include "keyboard.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Chip8 *chip = NULL;
static Uint64 last_time = 0;
static float sound_frequency = 1000.0f / 60.0f;
static uint64_t oplast = 0;
static SDL_AudioStream *stream = NULL;
static int current_sine_sample = 0;
static float opfrequency = (1.0f / 700.0f) * 1000.0f;

#define WINDOWS_WIDTH 64
#define WINDOWS_HEIGHT 32
#define SCALE_PX 12

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  SDL_AudioSpec spec;
  SDL_SetAppMetadata("Chip8", "1.0", "com.chip8");

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
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

  spec.channels = 1;
  spec.format = SDL_AUDIO_F32;
  spec.freq = 2000;
  stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec,
                                     NULL, NULL);
  if (!stream) {
    SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_ResumeAudioStreamDevice(stream);

  printf("rom route: %s\n", argv[1]);
  chip = initializeChip(argv);

  if (chip == NULL) {
    SDL_Log("failed to initialize chip: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  last_time = SDL_GetTicks();
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  uint8_t chipkey;
  switch (event->type) {
  case SDL_EVENT_KEY_DOWN:
    chipkey = mapkeycode(event->key.scancode);
    printf("[EVENT] key pressed: %d chip key: %d\n", event->key.scancode,
           chipkey);
    keydown(chip, chipkey);
    break;
  case SDL_EVENT_KEY_UP:
    chipkey = mapkeycode(event->key.scancode);
    printf("[EVENT] key released: %d chip key: %d\n", event->key.scancode,
           chipkey);
    keyup(chip, chipkey);
    break;
  case SDL_EVENT_QUIT:
    stop();
    free(chip);
    chip = NULL;
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

SDL_FRect rects[2048];
uint64_t now = 0;
SDL_AppResult SDL_AppIterate(void *appstate) {
  now = SDL_GetTicks();

  if (now > last_time + sound_frequency) {
    last_time = now;
    updatetimers(chip);

    if (chip->sound_t > 0) {
      static float samples[512];
      int i;

      for (i = 0; i < SDL_arraysize(samples); i++) {
        const int freq = 440;
        const float phase = current_sine_sample * freq / 2000.0f;
        samples[i] = SDL_sinf(phase * 2 * SDL_PI_F);
        current_sine_sample++;
      }

      current_sine_sample %= 2000;

      SDL_PutAudioStreamData(stream, samples, sizeof(samples));
    } else {
      SDL_ClearAudioStream(stream);
    }
  }
  
  float calc_elap = (now - oplast) / opfrequency;

  float modulo_calc = fmodf(calc_elap, 1.0f);
  if (calc_elap > 0 && (modulo_calc) == 0.0f) {
    printf("(%li - %li) / %f = %f\n",now, oplast, opfrequency, calc_elap);
    uint32_t opamount = (uint32_t)calc_elap;

    printf("operations = %d\n", opamount);
    oplast = now;
    cycle(chip, opamount);
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  for (int i = 0; i < SDL_arraysize(rects); i++) {
    rects[i].x = rects[i].y = rects[i].w = rects[i].h = 0;
  }

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

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRects(renderer, rects, SDL_arraysize(rects));

  SDL_RenderPresent(renderer);
  SDL_Delay(1);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  stop();
  free(chip);
  chip = NULL;
}
