#ifndef STEPPER_DRIVER_INIT_H
#define STEPPER_DRIVER_INIT_H

#include "stepper_driver_tmc2208.h"
#include "stepper_driver.h"
#include "esp_log.h"

extern stepper_driver_t *driver_L_handle, *driver_R_handle;
extern rmt_sync_manager_handle_t motor_sync_manager;

esp_err_t create_driver(void);
esp_err_t setup_driver(stepper_driver_t *stepper_handle);
esp_err_t create_sync_manager(void);

#endif // STEPPER_DRIVER_INIT_H