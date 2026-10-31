set(srcs)
set(include_dirs "include")

list(APPEND srcs
     )

idf_component_register(SRCS "tmc2208_uart_util.c" "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                       PRIV_REQUIRES esp_driver_uart esp_driver_gpio
                      )