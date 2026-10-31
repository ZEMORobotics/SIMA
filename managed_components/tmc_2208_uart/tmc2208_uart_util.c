#include "tmc2208_uart_util.h"

#define TAG "TMC2208_UART_UTIL"

esp_err_t tmc2208_uart_init(uint8_t port_num) {

  if (uart_is_driver_installed(port_num)) {
    ESP_LOGE(TAG, "Driver aleady installed, num: %d", port_num);
    uart_driver_delete(port_num);
    return ESP_FAIL;
  }

  uart_config_t tmc2208_uart_config = {
    .baud_rate = UART_BAUD,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 0,
    .source_clk = UART_SCLK_DEFAULT
  };

  if (uart_driver_install(port_num, 512, 0, 0, NULL, 0) != ESP_OK) {
    ESP_LOGE(TAG, "Driver installation failed, num: %d", port_num);
    return ESP_FAIL;
  }

  ESP_ERROR_CHECK(uart_param_config(port_num, &tmc2208_uart_config));
  switch (port_num) {
    case UART_NUM_0:
      ESP_ERROR_CHECK(uart_set_pin(port_num, GPIO_NUM_11, GPIO_NUM_12, 0, 0));
      break;
    case UART_NUM_1:
      ESP_ERROR_CHECK(uart_set_pin(port_num, GPIO_NUM_6, GPIO_NUM_7, 0, 0)); // Moguce obrnuto
      break;
    default:
      break;
  }
  ESP_ERROR_CHECK(uart_set_mode(port_num, UART_MODE_UART));
  
  return ESP_OK;
}

inline void uart_calcCRC(uint8_t *tx_packet, uint8_t packet_lenght) {
  uint8_t *crc = tx_packet + (packet_lenght - 1);
  uint8_t current_byte;

  *crc = 0;
  for (uint8_t i = 0; i < (packet_lenght - 1); i++) {
    current_byte = tx_packet[i];
    for (uint8_t j = 0; j < 8; j++) {
      if ((*crc >> 7) ^ (current_byte & 0x01))
        *crc = (*crc << 1) ^ 0x07;
      else 
        *crc = (*crc << 1);
      
      current_byte = current_byte >> 1;
    }
  }
}

esp_err_t tmc2208_uart_send(uint8_t port_num, uint8_t reg_addr, const uint32_t *const write_data, uint8_t rw) {
  uint8_t packet_lenght;

  if (rw) 
    packet_lenght = 8; 
  else 
    packet_lenght = 4;

  uint8_t tx_packet[packet_lenght];

  tx_packet[0] = 0x05;
  tx_packet[1] = 0x00;
  tx_packet[2] = (rw << 7) | reg_addr;
  ESP_LOGI(TAG, "UART send byte 3: %u", tx_packet[2]); 
  if (rw) {
    tx_packet[3] = (uint8_t) ((*write_data) & 0xf000);
    tx_packet[4] = (uint8_t) ((*write_data) & 0x0f00);
    tx_packet[5] = (uint8_t) ((*write_data) & 0x00f0);
    tx_packet[6] = (uint8_t) ((*write_data) & 0x000f);
  } 
  uart_calcCRC(tx_packet, packet_lenght);

  if (uart_write_bytes(port_num, tx_packet, packet_lenght) == ESP_FAIL) {
    ESP_LOGE(TAG, "UART write failed");
    return ESP_FAIL;
  }

  return ESP_OK;
}

esp_err_t tmc2208_uart_read(uint8_t port_num, uint8_t reg_addr, uint32_t *const read_data) {
  tmc2208_uart_send(port_num, reg_addr, NULL, READ);

  uint8_t buffer[8] = {0};

  if (uart_read_bytes(port_num, buffer, 8, pdMS_TO_TICKS(20)) < 0) {
    ESP_LOGE(TAG, "UART read failed");
    return ESP_FAIL;
  }

  *read_data = 0;
  *read_data |= (((uint32_t) buffer[3]) << 24);
  *read_data |= (((uint32_t) buffer[4]) << 16);
  *read_data |= (((uint32_t) buffer[5]) << 8);
  *read_data |= (uint32_t) buffer[6];

  return ESP_OK;
}