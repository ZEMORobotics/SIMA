#ifndef MOVE_H
#define MOVE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "move_util.h"

extern TaskHandle_t motors_task_handle;

esp_err_t translate_mm(int16_t motor_l_mm, int16_t motor_r_mm, bool accel, bool decel);
esp_err_t rotate_deg(int16_t deg, bool accel, bool decel);
esp_err_t stop_motors(void);

#endif // MOVE_H