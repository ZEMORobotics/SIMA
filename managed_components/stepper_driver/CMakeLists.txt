    # components/stepper_driver/CMakeLists.txt

    idf_component_register(SRCS "move_util.c" "stepper_motor_encoder.c" "stepper_driver_setup.c" "stepper_driver.c" "stepper_driver_tmc2208.c"
                        INCLUDE_DIRS "include"
                        REQUIRES esp_driver_gpio esp_driver_uart esp_driver_rmt)
					   
