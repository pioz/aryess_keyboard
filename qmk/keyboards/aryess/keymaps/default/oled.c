#if defined(OLED_ENABLE) && defined(WPM_ENABLE)
#include QMK_KEYBOARD_H
#include <stdio.h>
#if defined(POMODORO)
#include "pomodoro.h"
#endif

#define OLED_DISPLAY_128X32
#define IDLE_FRAMES 5
#define IDLE_SPEED 30
#define TAP_FRAMES 2
#define TAP_SPEED 40
#define ANIM_FRAME_DURATION 200
#define ANIM_SIZE 512

char wpm_str[16];
char caps_str[16];
char layer_str[16];
char pomodoro_str[16];
uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t current_idle_frame = 0;
uint8_t current_tap_frame = 0;

// Bongocat animation
//
// The entire display is divided into pages. Each page is 8 pixels high.
// Each column is 1 pixel wide. So: each byte = 1 column of 8 vertical
// pixels (4x8=32).
static void render_anim(void) {
  // Idle animation
  static const char PROGMEM idle[IDLE_FRAMES][ANIM_SIZE] = {
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,  16,   8,   8,   4,   4,   4,   8,  48,  64, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  24, 100, 130,   2,   2,   2,   2,   2,   1,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0, 192, 193, 193, 194,   4,   8,  16,  32,  64, 128,   0,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 192,  56,   4,   3,   0,   0,   0,   0,   0,   0,   0,  12,  12,  12,  13,   1,   0,  64, 160,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   8,   8,  16,  16,  16,  16,  16,  17,  15,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   2,   2,   4,   4,   8,   8,   8,   8,   8,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    },
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,  16,   8,   8,   4,   4,   4,   8,  48,  64, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  24, 100, 130,   2,   2,   2,   2,   2,   1,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0, 192, 193, 193, 194,   4,   8,  16,  32,  64, 128,   0,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 192,  56,   4,   3,   0,   0,   0,   0,   0,   0,   0,  12,  12,  12,  13,   1,   0,  64, 160,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   8,   8,  16,  16,  16,  16,  16,  17,  15,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   2,   2,   4,   4,   8,   8,   8,   8,   8,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    },
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128,  64,  64,  64,  64,  32,  32,  32,  32,  16,   8,   4,   2,   2,   4,  24,  96, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  60, 194,   1,   1,   2,   2,   4,   4,   2,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  96,  96,   0, 129, 130, 130, 132,   8,  16,  32,  64, 128,   0,   0,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 112,  25,   6,   0,   0,   0,   0,   0,   0,   0,  24,  24,  24,  27,   3,   0,  64, 160,  34,  36,  20,  18,  18,  18,  11,   8,   8,   8,   8,   5,   5,   9,   9,  16,  16,  16,  16,  16,  17,  15,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   2,   2,   4,   4,   8,   8,   8,   8,   8,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    },
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,   8,   4,   2,   1,   1,   2,  12,  48,  64, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  30, 225,   0,   0,   1,   1,   2,   2,   1,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0, 192, 193, 193, 194,   4,   8,  16,  32,  64, 128,   0,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 112,  12,   3,   0,   0,   0,   0,   0,   0,   0,  12,  12,  12,  13,   1,   0,  64, 160,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   8,   8,  16,  16,  16,  16,  16,  17,  15,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   2,   2,   4,   4,   8,   8,   8,   8,   8,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    },
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,   8,   8,   4,   2,   2,   2,   4,  56,  64, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  28, 226,   1,   1,   2,   2,   2,   2,   1,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0, 192, 193, 193, 194,   4,   8,  16,  32,  64, 128,   0,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 112,  12,   3,   0,   0,   0,   0,   0,   0,   0,  12,  12,  12,  13,   1,   0,  64, 160,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   8,   8,  16,  16,  16,  16,  16,  17,  15,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   2,   2,   4,   4,   8,   8,   8,   8,   8,   7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    }
  };

  // Prep animation
  static const char PROGMEM prep[][ANIM_SIZE] = {
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,   8,   4,   2,   1,   1,   2,  12,  48,  64, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  30, 225,   0,   0,   1,   1,   2,   2, 129, 128, 128,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0,   0,   1, 225,  26,   6,   9,  49,  53,   1, 138, 124,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 112,  12,   3,   0,   0,  24,   6,   5, 152, 153, 132, 195, 124,  65,  65,  64,  64,  32,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   4,   4,   4,   4,   2,   2,   2,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    }
  };

  // Typing animation
  static const char PROGMEM tap[TAP_FRAMES][ANIM_SIZE] = {
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,   8,   4,   2,   1,   1,   2,  12,  48,  64, 128,   0,   0,   0,   0,   0,   0,   0, 248, 248, 248, 248,   0,   0,   0,   0,   0, 128, 128,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  30, 225,   0,   0,   1,   1,   2,   2, 129, 128, 128,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0,   0,   1,   1,   2,   4,   8,  16,  32,  67, 135,   7,   1,   0, 184, 188, 190, 159,  95,  95,  79,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 112,  12,   3,   0,   0,  24,   6,   5, 152, 153, 132,  67, 124,  65,  65,  64,  64,  32,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   8,   8,  16,  16,  16,  16,  16,  17,  15,   1,  61, 124, 252, 252, 252, 252, 252,  60,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3,   2,   2,   1,   1,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   3,   3,   0,   0,
    },
    {
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0, 128,  64,  64,  32,  32,  32,  32,  16,  16,  16,  16,   8,   4,   2,   1,   1,   2,  12,  48,  64, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  30, 225,   0,   0,   1,   1,   2,   2,   1,   0,   0,   0,   0, 128, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,   0,  48,  48,   0,   0,   1, 225,  26,   6,   9,  49,  53,   1, 138, 124,   0,   0, 128, 128, 128, 128,  64,  64,  64,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 112,  12,   3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   0,  64, 160,  33,  34,  18,  17,  17,  17,   9,   8,   8,   8,   8,   4,   4,   4,   4,   4,   4,   2,   2,   2,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   8,   8,   8,   8,   8,   4,   4,   4,   4,   4,   2,   3, 122, 122, 121, 121, 121, 121,  57,  49,   2,   2,   4,   4,   8,   8,   8, 136, 136, 135, 128,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    }
  };

  void animation_phase(void) {
    if (get_current_wpm() <= IDLE_SPEED) {
      current_idle_frame = (current_idle_frame + 1) % IDLE_FRAMES;
      oled_write_raw_P(idle[abs((IDLE_FRAMES-1)-current_idle_frame)], ANIM_SIZE);
    }

    if (get_current_wpm() > IDLE_SPEED && get_current_wpm() < TAP_SPEED) {
      oled_write_raw_P(prep[0], ANIM_SIZE);
    }

    if (get_current_wpm() >= TAP_SPEED) {
      current_tap_frame = (current_tap_frame + 1) % TAP_FRAMES;
      oled_write_raw_P(tap[abs((TAP_FRAMES-1)-current_tap_frame)], ANIM_SIZE);
    }
  }

  if (get_current_wpm() != 000) {
    oled_on();

    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
      anim_timer = timer_read32();
      animation_phase();
    }

    anim_sleep = timer_read32();
  } else {
    if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
      oled_off();
    } else {
      if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animation_phase();
      }
    }
  }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  return OLED_ROTATION_180; // flips the display 180 degrees
}

char* get_caps_lock_status(void) {
  if (host_keyboard_led_state().caps_lock) {
    return "ON ";
  } else {
    return "OFF";
  }
}

char* get_rgblight_status(void) {
  if (rgblight_is_enabled()) {
    return "ON ";
  } else {
    return "OFF";
  }
}

char* get_current_layer(void) {
  uint8_t layer = get_highest_layer(layer_state | default_layer_state);
  switch (layer) {
    case FN1: return "FN1";
    case FN2: return "FN2";
    case WIN: return "WIN";
    case MAC: return "MAC";
    default: return "?";
  }
}

bool oled_task_user(void) {
  render_anim();

  oled_set_cursor(0, 0);
  sprintf(wpm_str, "WPM:%03d", get_current_wpm());
  oled_write(wpm_str, false);

  oled_set_cursor(0, 1);
  sprintf(caps_str, "CAP:%s", get_caps_lock_status());
  oled_write(caps_str, false);

  oled_set_cursor(0, 2);
  sprintf(layer_str, "LYR:%s", get_current_layer());
  oled_write(layer_str, false);

  #if defined(POMODORO)
  oled_set_cursor(0, 3);
  sprintf(pomodoro_str, "PMD:%s", pomodoro_get_status());
  oled_write(pomodoro_str, false);
  #endif

  return false;
}

#endif
