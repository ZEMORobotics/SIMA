#include "move.h"
#include "stepper_driver_setup.h"
#include "math.h"

#define MM_TO_STEPS(x) ((int32_t) roundf(x / (roundf(WHEEL_CIR / STEPS_PER_REV))) * MICRO_PER_STEP) 

esp_err_t translate_mm(int16_t motor_l_mm, int16_t motor_r_mm, bool accel, bool decel) {

  int32_t L_steps, R_steps;

  L_steps = MM_TO_STEPS(motor_l_mm);
  R_steps = MM_TO_STEPS(motor_r_mm);

  ESP_ERROR_CHECK(tmc2208_move_sync(&motor_cfg, L_steps, R_steps, accel, decel));

  return ESP_OK;
}

esp_err_t rotate_deg(int16_t deg, bool accel, bool decel) {

  int32_t rot_steps;

  deg = (int16_t) roundf(M_PI * (deg / 180.0));

  rot_steps = MM_TO_STEPS(roundf((WHEEL_DIST / 2) * abs(deg)));

  if (deg > 0)
    ESP_ERROR_CHECK(tmc2208_move_sync(&motor_cfg, -rot_steps, rot_steps, accel, decel));
  else
    ESP_ERROR_CHECK(tmc2208_move_sync(&motor_cfg, rot_steps, -rot_steps, accel, decel));

  return ESP_OK;
}

esp_err_t stop_motors(void) {

  ESP_ERROR_CHECK(tmc2208_move_disable(driver_L_handle, true));
  ESP_ERROR_CHECK(tmc2208_move_disable(driver_R_handle, true));

  return ESP_OK;
}