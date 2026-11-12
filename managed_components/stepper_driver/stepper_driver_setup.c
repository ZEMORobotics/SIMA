#include "stepper_driver_setup.h"

#define TAG "stepper_driver_setup"
#define MICROSTEPS MICROSTEPS_16
#define I_RUN 300 // mA
#define I_HOLD 50 // percent of I_RUN
#define T_OFF 4 // ms
#define I_HOLD_DELAY 6 // ms
#define T_POWER_DOWN 20 // ms

#define MOTORS_NUM 2

stepper_driver_t *driver_L_handle = NULL, *driver_R_handle = NULL;
rmt_sync_manager_handle_t motor_sync_manager = NULL;

esp_err_t create_driver(void) {

  stepper_driver_tmc2208_conf_t driver_L_conf = {
    .uart_port = UART_NUM_1,
    .rx_pin = GPIO_NUM_7,
    .tx_pin = GPIO_NUM_6,
    .baud_rate = 115200,
    .step_pin = GPIO_NUM_3,
    .direction_pin = GPIO_NUM_2,
    .enable_pin = GPIO_NUM_10
  };

  stepper_driver_tmc2208_conf_t driver_R_conf = {
      .uart_port = UART_NUM_0,
      .rx_pin = GPIO_NUM_12,
      .tx_pin = GPIO_NUM_11,
      .baud_rate = 115200,
      .step_pin = GPIO_NUM_4,
      .direction_pin = GPIO_NUM_8,
      .enable_pin = GPIO_NUM_5
  };

  driver_L_handle = stepper_driver_new_tmc2208(&driver_L_conf);
  driver_R_handle = stepper_driver_new_tmc2208(&driver_R_conf);

  if (driver_L_handle == NULL || driver_R_handle == NULL) {
    ESP_LOGE(TAG, "Error creating stepper driver handles");
    return ESP_FAIL;
  }

  if (stepper_driver_init(driver_L_handle) != ESP_OK || stepper_driver_init(driver_R_handle) != ESP_OK) {
    ESP_LOGE(TAG, "Error initializing stepper driver handles");
    return ESP_FAIL;
  }

  stepper_driver_clear_gstat(driver_L_handle);
  stepper_driver_clear_gstat(driver_R_handle);

  return ESP_OK;
}

esp_err_t setup_driver(stepper_driver_t *stepper_handle) {

  stepper_driver_set_toff(stepper_handle, T_OFF);
  stepper_driver_set_microsteps_per_step(stepper_handle, MICROSTEPS);
  stepper_driver_enable_pwm_autoscale(stepper_handle);
  stepper_driver_set_current(stepper_handle, I_RUN, I_HOLD);
  stepper_driver_set_iholdddelay(stepper_handle, I_HOLD_DELAY);
  stepper_driver_set_tpowerdown(stepper_handle, T_POWER_DOWN);

  uint8_t ifcnt = (uint8_t) stepper_driver_read_register_ifcnt(stepper_handle);
  uint8_t version = (uint8_t) (stepper_driver_read_register_ioin(stepper_handle) >> 24);

  if (ifcnt > 0 && version == 0x20)
    return ESP_OK;
  else
    return ESP_FAIL;
}

esp_err_t create_sync_manager(void) {

  stepper_driver_tmc2208_t *driver_L_tmc = __containerof(driver_L_handle, stepper_driver_tmc2208_t, parent);
  stepper_driver_tmc2208_t *driver_R_tmc = __containerof(driver_R_handle, stepper_driver_tmc2208_t, parent);

  rmt_channel_handle_t arr[MOTORS_NUM] = {driver_L_tmc->rmt_channel, driver_R_tmc->rmt_channel};

  rmt_sync_manager_config_t motor_sync_manager_cfg = {
    .tx_channel_array = arr,
    .array_size = MOTORS_NUM
  };

  ESP_ERROR_CHECK(rmt_enable(driver_L_tmc->rmt_channel));
  ESP_ERROR_CHECK(rmt_enable(driver_R_tmc->rmt_channel));

  ESP_ERROR_CHECK(rmt_new_sync_manager(&motor_sync_manager_cfg, &motor_sync_manager));

  return ESP_OK;
}