RGB_MATRIX_EFFECT(PC_DRIVEN)
RGB_MATRIX_EFFECT(EMPTY)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool PC_DRIVEN(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  return rgb_matrix_check_finished_leds(led_max);
}
static bool EMPTY(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  for (uint8_t i = led_min; i < led_max; i++) {
    rgb_matrix_set_color(i, 0,0,0);//i % 255, i*2 % 255, i*3 % 255);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
