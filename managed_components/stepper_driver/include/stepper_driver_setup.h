#ifndef STEPPER_DRIVER_INIT_H
#define STEPPER_DRIVER_INIT_H

#include "stepper_driver_tmc2208.h"
#include "stepper_driver.h"
#include "esp_log.h"

esp_err_t create_driver(stepper_driver_t **L_driver_handle, stepper_driver_t **R_driver_handle);
esp_err_t setup_driver(stepper_driver_t *stepper_handle);

#endif // STEPPER_DRIVER_INIT_H