#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "map.h"
#include "path_finder.h"
#include "inflation_layer.h"
#include "stepper_driver_setup.h"
#include "move.h"
#include "esp_log.h"
#include "path_follow.h"

#define TAG "main"

void app_main(void)
{
    map_init();

    //print_path(NULL, 0);

    uint8_t ri, ci, re, ce;
    int16_t *path = NULL, len = 0;

    apply_inflation(CONFIG_INFLATION_DEPTH);

    /*printf("\n\n\n");
    for (int16_t i = MAP_SIZE - 1; i >= 0; --i)
    {
        printf("%d ", pinf[i]);   
        if (!(i % ROW_SIZE))
            printf("\n");
    }
    */

    if (create_driver() != ESP_OK) {
        printf("Driver creation failed!\n");
    }

    if (setup_driver(driver_L_handle) == ESP_OK) {
        printf("OK bajo imamo nesto!\n");
    }
    else
        printf("Vise srece drugi put\n");

    if (create_sync_manager() != ESP_OK) 
        printf("Error creating sync\n");
    
    stepper_driver_dump_register_pwmconf(driver_L_handle);
    stepper_driver_dump_register_chopconf(driver_L_handle);

    srand(time(0));
    while (1)
    {
        ri = rand() % NUMBER_OF_ROWS;
        ci = rand() % ROW_SIZE;
        re = rand() % NUMBER_OF_ROWS;
        ce = rand() % ROW_SIZE;
        vTaskDelay(250 / portTICK_PERIOD_MS);
        set_init_num(ci, ri);
        set_end_num(ce, re);
        if (path_find() >= 0)
        {
            printf("Found something for now. Start node %d, end node %d\n", XY_TO_NODE(ci, ri), XY_TO_NODE(ce, re));
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            path = reconstruct_path(&len);
            if (path != NULL)
            {
                //print_path(path, &len);
                //for (--len; len >= 0; --len)
                    //printf("%d\n", path[len]);

                free(path);
                path = NULL;

                //ESP_LOGI(TAG, "Odmah pre ponovnog kretanja");
                //if (tmc2208_move_enable(driver_L_handle, &motor_cfg) == ESP_OK) {
                    //vTaskDelay(pdMS_TO_TICKS(1000));
                    //tmc2208_move_steps(driver_L_handle, &motor_cfg, 4400, true, true);
                    //tmc2208_move_steps(driver_L_handle, &motor_cfg, -4400, true, true);
                    //tmc2208_move_sync(&motor_cfg, 4400, -4400, true, true);
                    translate_mm(50*M_PI, 50*M_PI, true, true);
                    //rotate_deg(-90, false, false);
                    ESP_LOGI(TAG, "MSCNT: %ld", stepper_driver_read_register_mscntd(driver_L_handle));
                    //tmc2208_uniform_sync_move(&motor_cfg, 4400, 4400);
                    //tmc2208_move_steps(driver_L_handle, &motor_cfg, 4400, true, true);
                //}
            }
        }
        else
        {
            printf("Too long\n");
        }
    }
}