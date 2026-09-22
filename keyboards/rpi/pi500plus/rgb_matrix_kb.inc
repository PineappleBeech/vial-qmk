/* Copyright 2025 Raspberry Pi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef RPI_RGB_STARTUP_ANIMATION
RGB_MATRIX_EFFECT(STARTUP_ANIM_W)
RGB_MATRIX_EFFECT(STARTUP_ANIM_B)
RGB_MATRIX_EFFECT(SHUTDOWN_ANIM_W)
RGB_MATRIX_EFFECT(SHUTDOWN_ANIM_B)
RGB_MATRIX_EFFECT(IDLE_PROGRESS_BAR)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "pi500plus.h"
#include "rpi.h"

#define STARTUP_ANIM_FRAMES 120
#define SHUTDOWN_ANIM_FRAMES 120
#if defined(KEYBOARD_rpi_pi500plus_iso)
#	include "startup_anim/Pulse_Black_ISO.h"
#	include "startup_anim/Pulse_White_ISO.h"
#endif
#if defined(KEYBOARD_rpi_pi500plus_ansi)
#	include "startup_anim/Pulse_Black_ANSI.h"
#	include "startup_anim/Pulse_White_ANSI.h"
#endif
#if defined(KEYBOARD_rpi_pi500plus_jis)
#	include "startup_anim/Pulse_Black_JIS.h"
#	include "startup_anim/Pulse_White_JIS.h"
#endif

#define ANIM_INTERVAL 12

static bool STARTUP_ANIM_W(effect_params_t* params) {
  static uint8_t frame;
  static uint32_t wait_timer;
  static bool frame_complete;
  static bool animation_complete;

  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  inline void update_leds(uint8_t new_frame) {
    for (int n = led_min; n < led_max; n++) {
      HSV hsv = {h_values_1[new_frame][n], s_values_1[new_frame][n], scale8(v_values_1[new_frame][n],rgb_matrix_config.hsv.v)};
      RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
      rgb_matrix_set_color(n, rgb.r, rgb.g, rgb.b);
    }
  }

  if (params->init) {
    if (led_min == 0) { // Only run once to save cycles
      // memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
      frame = 0;
      frame_complete = true;
      animation_complete = false;
      wait_timer = g_rgb_timer + ANIM_INTERVAL;
    }
  }

  if (!animation_complete) {
    if (!frame_complete) {
      update_leds(frame);
      if (!rgb_matrix_check_finished_leds(led_max)) {
        frame++;
        wait_timer = g_rgb_timer + ANIM_INTERVAL;
        frame_complete = true;
        return false;
      }
      else {
        return true;
      }
    }
    else if (g_rgb_timer > wait_timer) {
      if (frame<STARTUP_ANIM_FRAMES) {
        frame_complete = false;
        update_leds(frame);
        if (!rgb_matrix_check_finished_leds(led_max)) {
          frame++;
          wait_timer = g_rgb_timer + ANIM_INTERVAL;
          frame_complete = true;
          return false;
        }
        else {
          return true;
        }
      }
      else {
        animation_complete = true;
        dprintf("Animation Complete - Return to EEPROM stored RGB mode\n");
        rpi_rgb_matrix_startup_callback(RGB_MATRIX_CUSTOM_STARTUP_ANIM_W);
        return false;
      }
    }
  }

  return rgb_matrix_check_finished_leds(led_max); // Returns false if led_max is the final led in the matrix
}

static bool STARTUP_ANIM_B(effect_params_t* params) {
  static uint8_t frame;
  static uint32_t wait_timer;
  static bool frame_complete;
  static bool animation_complete;

  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  inline void update_leds(uint8_t new_frame) {
    for (int n = led_min; n < led_max; n++) {
      HSV hsv = {h_values_0[new_frame][n], s_values_0[new_frame][n], scale8(v_values_0[new_frame][n],rgb_matrix_config.hsv.v)};
      RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
      rgb_matrix_set_color(n, rgb.r, rgb.g, rgb.b);
    }
  }

  if (params->init) {
    if (led_min == 0) { // Only run once to save cycles
      // memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
      frame = 0;
      frame_complete = true;
      animation_complete = false;
      wait_timer = g_rgb_timer + ANIM_INTERVAL;
    }
  }

  if (!animation_complete) {
    if (!frame_complete) {
      update_leds(frame);
      if (!rgb_matrix_check_finished_leds(led_max)) {
        frame++;
        wait_timer = g_rgb_timer + ANIM_INTERVAL;
        frame_complete = true;
        return false;
      }
      else {
        return true;
      }
    }
    else if (g_rgb_timer > wait_timer) {
      if (frame<STARTUP_ANIM_FRAMES) {
        frame_complete = false;
        update_leds(frame);
        if (!rgb_matrix_check_finished_leds(led_max)) {
          frame++;
          wait_timer = g_rgb_timer + ANIM_INTERVAL;
          frame_complete = true;
          return false;
        }
        else {
          return true;
        }
      }
      else {
        animation_complete = true;
        dprintf("Animation Complete - Return to EEPROM stored RGB mode\n");
        rpi_rgb_matrix_startup_callback(RGB_MATRIX_CUSTOM_STARTUP_ANIM_B);
        return false;
      }
    }
  }

  return rgb_matrix_check_finished_leds(led_max); // Returns false if led_max is the final led in the matrix
}

static bool SHUTDOWN_ANIM_W(effect_params_t* params) {
  static uint8_t frame;
  static uint32_t wait_timer;
  static bool frame_complete;
  static bool animation_complete;

  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  inline void update_leds(uint8_t new_frame) {
    for (int n = led_min; n < led_max; n++) {
      uint8_t idx = SHUTDOWN_ANIM_FRAMES - 1 - new_frame;
      HSV hsv = {h_values_1[idx][n], s_values_1[idx][n], scale8(v_values_1[idx][n],rgb_matrix_config.hsv.v)};
      RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
      rgb_matrix_set_color(n, rgb.r, rgb.g, rgb.b);
    }
  }

  if (params->init) {
    if (led_min == 0) { // Only run once to save cycles
      // memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
      frame = 0;
      frame_complete = true;
      animation_complete = false;
      wait_timer = g_rgb_timer + ANIM_INTERVAL;
    }
  }

  if (!animation_complete) {
    if (!frame_complete) {
      update_leds(frame);
      if (!rgb_matrix_check_finished_leds(led_max)) {
        frame++;
        wait_timer = g_rgb_timer + ANIM_INTERVAL;
        frame_complete = true;
        return false;
      }
      else {
        return true;
      }
    }
    else if (g_rgb_timer > wait_timer) {
      if (frame<SHUTDOWN_ANIM_FRAMES) {
        frame_complete = false;
        update_leds(frame);
        if (!rgb_matrix_check_finished_leds(led_max)) {
          frame++;
          wait_timer = g_rgb_timer + ANIM_INTERVAL;
          frame_complete = true;
          return false;
        }
        else {
          return true;
        }
      }
      else {
        animation_complete = true;
        dprintf("Shutdown Animation Complete - Starting idle animation\n");
        rpi_rgb_matrix_shutdown_callback(RGB_MATRIX_CUSTOM_SHUTDOWN_ANIM_W);
        return false;
      }
    }
  }

  return rgb_matrix_check_finished_leds(led_max); // Returns false if led_max is the final led in the matrix
}

static bool SHUTDOWN_ANIM_B(effect_params_t* params) {
  static uint8_t frame;
  static uint32_t wait_timer;
  static bool frame_complete;
  static bool animation_complete;

  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  inline void update_leds(uint8_t new_frame) {
    for (int n = led_min; n < led_max; n++) {
      uint8_t idx = SHUTDOWN_ANIM_FRAMES - 1 - new_frame;
      HSV hsv = {h_values_0[idx][n], s_values_0[idx][n], scale8(v_values_0[idx][n],rgb_matrix_config.hsv.v)};
      RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
      rgb_matrix_set_color(n, rgb.r, rgb.g, rgb.b);
    }
  }

  if (params->init) {
    if (led_min == 0) { // Only run once to save cycles
      // memset(g_rgb_frame_buffer, 0, sizeof g_rgb_frame_buffer);
      frame = 0;
      frame_complete = true;
      animation_complete = false;
      wait_timer = g_rgb_timer + ANIM_INTERVAL;
    }
  }

  if (!animation_complete) {
    if (!frame_complete) {
      update_leds(frame);
      if (!rgb_matrix_check_finished_leds(led_max)) {
        frame++;
        wait_timer = g_rgb_timer + ANIM_INTERVAL;
        frame_complete = true;
        return false;
      }
      else {
        return true;
      }
    }
    else if (g_rgb_timer > wait_timer) {
      if (frame<SHUTDOWN_ANIM_FRAMES) {
        frame_complete = false;
        update_leds(frame);
        if (!rgb_matrix_check_finished_leds(led_max)) {
          frame++;
          wait_timer = g_rgb_timer + ANIM_INTERVAL;
          frame_complete = true;
          return false;
        }
        else {
          return true;
        }
      }
      else {
        animation_complete = true;
        dprintf("Shutdown Animation Complete - Starting idle animation\n");
        rpi_rgb_matrix_shutdown_callback(RGB_MATRIX_CUSTOM_SHUTDOWN_ANIM_B);
        return false;
      }
    }
  }

  return rgb_matrix_check_finished_leds(led_max); // Returns false if led_max is the final led in the matrix
}

#define RPI_KEY_HALF_WIDTH  16           // must be a power of two
#define RPI_KEY_FADE_SHIFT  5            // log2(2 * RPI_KEY_HALF_WIDTH) for dividing
#define RPI_KEYBOARD_WIDTH 224

static bool IDLE_PROGRESS_BAR(effect_params_t* params) {
  static uint32_t duration;
  static uint8_t edge;
  static bool running;


  RGB_MATRIX_USE_LIMITS(led_min, led_max);

  inline void update_leds(void) {
    for (int n = led_min; n < led_max; n++) {
      HSV hsv = rgb_matrix_config.hsv;
      uint8_t x = g_led_config.point[n].x;
      if (x >= (edge + RPI_KEY_HALF_WIDTH) || !running) {
        hsv.v = 0;
      } else if (x > (edge - RPI_KEY_HALF_WIDTH)) {
        hsv.v = (uint8_t)(((uint16_t)hsv.v * (uint16_t)(edge + RPI_KEY_HALF_WIDTH - x)) >> RPI_KEY_FADE_SHIFT);
      }
      RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
      rgb_matrix_set_color(n, rgb.r, rgb.g, rgb.b);
    }
  }

  // Cannot call init here

  if (led_min == 0) {
    duration = rpi_get_idle_duration();
    if (duration == 0) {
      running = false;
    } else {
      uint32_t start_time = rpi_get_idle_duration_start();
      uint64_t elapsed = sync_timer_read32() - start_time;
      if (elapsed > duration) {
        elapsed = duration;
      }
      uint32_t edge32 = (elapsed * (RPI_KEYBOARD_WIDTH + RPI_KEY_HALF_WIDTH)) / duration;
      edge = edge32 < 255 ? edge32 : 255;
      running = true;
    }
  }
  update_leds();

  return rgb_matrix_check_finished_leds(led_max);
}

#endif // #ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // #ifdef RPI_RGB_STARTUP_ANIMATION
