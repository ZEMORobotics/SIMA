#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tmc2208.h"
#include "path_finding.h"

void app_main(void)
{
    set_init_node(39, 59);
    set_end_node(0, 0);
    map_init();

    for (int16_t i = MAP_SIZE - 1; i >= 0; --i) {
        printf("%d ", get_f_value(i));
        if (!(i % ROW_SIZE))
            printf("\n");
    }

    while (1)
    {
        printf("Calculated heuristic\n");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    
}