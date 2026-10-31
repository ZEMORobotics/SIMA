#ifndef TMC2208_UART_UTIL_H
#define TMC2208_UART_UTIL_H

#include "driver/gpio.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_intr_alloc.h"

#define R_DRIVER_UART UART_NUM_0
#define L_DRIVER_UART UART_NUM_1

#define R_DRIVER_EN   GPIO_NUM_5
#define R_DRIVER_STEP GPIO_NUM_4
#define R_DRIVER_DIR  GPIO_NUM_8
#define L_DRIVER_EN   GPIO_NUM_10
#define L_DIRVER_STEP GPIO_NUM_3
#define L_DRIVER_DIR  GPIO_NUM_2

#define UART_BAUD 115200

#define READ 0
#define WRITE 1

esp_err_t tmc2208_uart_init(uint8_t port_num);
esp_err_t tmc2208_uart_send(uint8_t port_num, uint8_t reg_addr, const uint32_t *const write_data, uint8_t rw);
void uart_calcCRC(uint8_t *tx_packet, uint8_t packet_lenght);
esp_err_t tmc2208_uart_read(uint8_t port_num, uint8_t reg_addr, uint32_t *const read_data);

#endif  // TMC2208_UART_UTIL_H