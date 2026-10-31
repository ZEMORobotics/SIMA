#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "map.h"
#include "path_finder.h"
#include "inflation_layer.h"
#include "tmc2208_uart_util.h"

void app_main(void)
{
    map_init();

    print_path(NULL, 0);

    uint8_t ri, ci, re, ce;
    int16_t *path = NULL, len = 0;

    apply_inflation(CONFIG_INFLATION_DEPTH);

    printf("\n\n\n");
    for (int16_t i = MAP_SIZE - 1; i >= 0; --i)
    {
        printf("%d ", pinf[i]);   
        if (!(i % ROW_SIZE))
            printf("\n");
    }
    
    if (tmc2208_uart_init(L_DRIVER_UART) == ESP_OK)
        printf("L_DRIVER uart init success\n");
    if (tmc2208_uart_init(R_DRIVER_UART) == ESP_OK)
        printf("R_DRIVER uart init success\n");

    uint32_t recv = 0;

    if (tmc2208_uart_read(L_DRIVER_UART, 0x70, &recv) == ESP_OK) {
        printf("Read successful! You've read: %lx\n", recv);
    } else
        printf("Read error!\n");
    if (tmc2208_uart_read(R_DRIVER_UART, 0x70, &recv) == ESP_OK) {
        printf("Read successful! You've read: %lx\n", recv);
    } else
        printf("Read error!\n");

    if (tmc2208_uart_send(L_DRIVER_UART, 0x70, &recv, 1) == ESP_OK) {
        printf("Write succeded! Now read.\n");
        if (tmc2208_uart_read(L_DRIVER_UART, 0x70, &recv) == ESP_OK) {
            printf("Read successful! You've read: %lx\n", recv);
        } else
            printf("Read error!\n");
    }

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
                print_path(path, &len);
                for (--len; len >= 0; --len)
                    printf("%d\n", path[len]);

                free(path);
                path = NULL;
            }
        }
        else
        {
            printf("Too long.\n");
        }
    }
}