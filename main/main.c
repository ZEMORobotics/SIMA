#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tmc2208.h"
#include "map.h"
#include "path_finder.h"

void app_main(void)
{
    set_init_node(39, 59);
    set_end_node(12, 34);
    map_init();

    for (int16_t i = MAP_SIZE - 1; i >= 0; --i) {
        printf("%d ", get_f_value(i));
        if (!(i % ROW_SIZE))
            printf("\n");
    }

    if (path_find()) {
        printf("Found something for now.\n");
    } else {
        printf("Too long.\n");
    }

    uint8_t flag = 1;
    while (1)
    {
        vTaskDelay(10000 / portTICK_PERIOD_MS);
        if (flag) {
            flag = 0;
            set_init_node(15, 24);
            set_end_node(12, 34);
            if (path_find()) {
                printf("Found something for now.\n");
            } else {
                printf("Too long.\n");
            }
        }
        printf("Test\n");
    }
    
}