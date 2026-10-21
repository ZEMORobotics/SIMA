#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tmc2208.h"
#include "map.h"
#include "path_finder.h"

void app_main(void)
{
    set_end_node(12, 34);
    set_init_node(15, 24);
    map_init();

    for (int16_t i = MAP_SIZE - 1; i >= 0; --i)
    {
        printf("%d ", get_f_value(i));
        if (!(i % ROW_SIZE))
            printf("\n");
    }

    uint8_t r, c;
    int16_t *path = NULL, len = 0;

    srand(time(0));
    while (1)
    {
        r = rand() % NUMBER_OF_ROWS;
        c = rand() % ROW_SIZE;
        vTaskDelay(250 / portTICK_PERIOD_MS);
        set_init_node(c, r);
        if (path_find())
        {
            printf("Found something for now. Start node %d, end node %d\n", XY_TO_NODE(c, r), XY_TO_NODE(12, 34));
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            path = reconstruct_path(&len);
            if (path != NULL)
            {
                printf("Follow the path:\n");
                for (--len; len >= 0; --len)
                    printf("%d\n", path[len]);

                free(path);
            }
        }
        else
        {
            printf("Too long.\n");
        }
    }
}