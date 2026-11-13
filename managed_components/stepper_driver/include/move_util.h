#ifndef MOVE_UTIL_H
#define MOVE_UTIL_H

#include <stdint.h>
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "stepper_driver_tmc2208.h"
#include "stepper_driver.h"
#include "esp_err.h"
#include "math.h"

#define MICRO_PER_STEP 16
#define STEPS_PER_REV 200
#define WHEEL_DMR 50.0 // Obavezno stavi sa tackom, MM
#define WHEEL_CIR 50.0 * M_PI // Obavezno stavi sa tackom
#define WHEEL_DIST 90.0

typedef enum {TMC2208_DIR_CW = 0, TMC2208_DIR_CCW} tmc2208_dir_t;
typedef enum {TMC2208_CURVE_SMOOTH = 0, TMC2208_CURVE_LINEAR} tmc2208_curve_t;

typedef struct {
    uint32_t resolution_hz;              /**< Resolution in Hz. */
    uint32_t start_freq_hz;              /**< Starting frequency in Hz. */
    uint32_t end_freq_hz;                /**< Ending frequency in Hz. */
    uint32_t accel_samples;              /**< Number of samples for acceleration. */
    uint32_t uniform_speed_hz;           /**< Uniform speed in Hz. */
    uint32_t decel_samples;              /**< Number of samples for deceleration. */
    uint32_t max_freq_hz;                /**< Maximum frequency in Hz. */
} tmc2208_motor_config_t;

extern tmc2208_motor_config_t motor_cfg;

esp_err_t tmc2208_move_enable(stepper_driver_t *handle, bool is_sync);
esp_err_t tmc2208_move_disable(stepper_driver_t *handle, bool is_sync);
esp_err_t tmc2208_move_steps(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config, int32_t steps, bool accel, bool decel);
esp_err_t tmc2208_uniform_move_steps(stepper_driver_t *handle, tmc2208_motor_config_t *motor_config, int32_t steps);
esp_err_t tmc2208_uniform_sync_move(tmc2208_motor_config_t *motor_config, int32_t steps_L, int32_t steps_R);
esp_err_t tmc2208_move_sync(tmc2208_motor_config_t *motor_config, int32_t steps_L, int32_t steps_R, bool accel, bool decel);

#endif // MOVE_UTIL_H