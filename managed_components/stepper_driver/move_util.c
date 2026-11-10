#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/cdefs.h>

#include "move_util.h"
#include "stepper_motor_encoder.h"

static const char *TAG = "MOVE_UTIL";

static esp_err_t create_uniform_motor_encoder(const tmc2208_motor_config_t *motor_config, rmt_encoder_handle_t *uniform_motor_encoder)
{
  stepper_motor_uniform_encoder_config_t uniform_encoder_config = {
      .resolution = motor_config->resolution_hz,
  };

  *uniform_motor_encoder = NULL;
  rmt_new_stepper_motor_uniform_encoder(&uniform_encoder_config, uniform_motor_encoder);

  return ESP_OK;
}

static esp_err_t tmc2208_accelerate(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config)
{
  stepper_driver_tmc2208_t *tmc2208 = __containerof(handle, stepper_driver_tmc2208_t, parent);

  stepper_motor_curve_encoder_config_t accel_encoder_config = {
      .resolution = motor_config->resolution_hz,
      .sample_points = motor_config->accel_samples,
      .start_freq_hz = motor_config->start_freq_hz,
      .end_freq_hz = motor_config->uniform_speed_hz,
  };

  rmt_encoder_handle_t accel_motor_encoder = NULL;
  ESP_ERROR_CHECK(rmt_new_stepper_motor_curve_encoder(&accel_encoder_config, &accel_motor_encoder));

  rmt_transmit_config_t tx_config = {
      .loop_count = 0,
  };

  // acceleration phase
  tx_config.loop_count = 0;
  ESP_ERROR_CHECK(rmt_transmit(tmc2208->rmt_channel, accel_motor_encoder, &motor_config->accel_samples, sizeof(motor_config->accel_samples), &tx_config));
  // wait all transactions finished
  ESP_ERROR_CHECK(rmt_tx_wait_all_done(tmc2208->rmt_channel, -1));

  return ESP_OK;
}

static esp_err_t tmc2208_decelerate(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config)
{
  stepper_driver_tmc2208_t *tmc2208 = __containerof(handle, stepper_driver_tmc2208_t, parent);

  stepper_motor_curve_encoder_config_t decel_encoder_config = {
      .resolution = motor_config->resolution_hz,
      .sample_points = motor_config->decel_samples,
      .start_freq_hz = motor_config->uniform_speed_hz,
      .end_freq_hz = motor_config->end_freq_hz,
  };
  rmt_encoder_handle_t decel_motor_encoder = NULL;
  ESP_ERROR_CHECK(rmt_new_stepper_motor_curve_encoder(&decel_encoder_config, &decel_motor_encoder));

  rmt_transmit_config_t tx_config = {
      .loop_count = 0,
  };

  // deceleration phase
  tx_config.loop_count = 0;
  ESP_ERROR_CHECK(rmt_transmit(tmc2208->rmt_channel, decel_motor_encoder, &motor_config->decel_samples, sizeof(motor_config->decel_samples), &tx_config));
  // wait all transactions finished
  ESP_ERROR_CHECK(rmt_tx_wait_all_done(tmc2208->rmt_channel, -1));

  return ESP_OK;
}

esp_err_t tmc2208_uniform_move_steps(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config, int32_t steps)
{
  
  stepper_driver_tmc2208_t *tmc2208 = __containerof(handle, stepper_driver_tmc2208_t, parent);

  rmt_symbol_word_t *items = (rmt_symbol_word_t*) pvPortMalloc(steps * sizeof(rmt_symbol_word_t));

  if (!items) {
      ESP_LOGE(TAG, "Failed to allocate memory for RMT items");
      return ESP_ERR_NO_MEM;
  }

  uint16_t signal_duration = motor_config->resolution_hz / motor_config->uniform_speed_hz / 2;

  for (uint32_t i = 0; i < steps; i++) {
      items[i].level0 = 1;
      items[i].duration0 = signal_duration;
      items[i].level1 = 0;
      items[i].duration1 = signal_duration;
  }
    
  rmt_transmit_config_t txConfig = {};
  txConfig.loop_count = 0;

  ESP_ERROR_CHECK(rmt_transmit(
        tmc2208->rmt_channel,
        tmc2208->rmt_encoder,
        items,
        steps * sizeof(rmt_symbol_word_t),
        &txConfig
    ));

  vPortFree(items);
  ESP_ERROR_CHECK(rmt_tx_wait_all_done(tmc2208->rmt_channel, -1));
  
  return ESP_OK;
  
}

esp_err_t tmc2208_move_steps(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config, int32_t steps, bool accel, bool decel)
{

  stepper_driver_tmc2208_t *tmc2208 = __containerof(handle, stepper_driver_tmc2208_t, parent);

  // Enable motor
  tmc2208_move_enable(handle, motor_config);

  // Set direction
  if (steps > 0) {
    stepper_driver_direction(handle, TMC2208_DIR_CW);
  }
  else {
    stepper_driver_direction(handle, TMC2208_DIR_CCW);
    steps = steps * -1;
  }

  if (accel) {
    tmc2208_accelerate(handle, motor_config);
  }

  tmc2208_uniform_move_steps(handle, motor_config, steps);

  if (decel) {
    tmc2208_decelerate(handle, motor_config);
  }

  // Disable motor
  tmc2208_move_disable(handle, motor_config);

  return ESP_OK;
}

esp_err_t tmc2208_move_enable(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config)
{
  stepper_driver_tmc2208_t *tmc2208 = __containerof(handle, stepper_driver_tmc2208_t, parent);

  // Set enable to 0
  stepper_driver_enable(handle);

  // Set direction
  stepper_driver_direction(handle, TMC2208_DIR_CW);
  //ESP_ERROR_CHECK(create_uniform_motor_encoder(motor_config, &tmc2208->rmt_encoder));
  ESP_ERROR_CHECK(rmt_enable(tmc2208->rmt_channel));
  return ESP_OK;
}

esp_err_t tmc2208_move_disable(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config)
{
  stepper_driver_tmc2208_t *tmc2208 = __containerof(handle, stepper_driver_tmc2208_t, parent);

  // Set enable to 1
  stepper_driver_disable(handle);
  ESP_ERROR_CHECK(rmt_disable(tmc2208->rmt_channel));
  //ESP_ERROR_CHECK(rmt_del_encoder(tmc2208->rmt_encoder));
  return ESP_OK;
}
