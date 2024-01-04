
RGB_MATRIX_EFFECT(PC_DRIVEN)


#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
static bool PC_DRIVEN(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  return rgb_matrix_check_finished_leds(led_max);
}
#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
