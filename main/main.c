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
    set_end_num(12, 34);
    set_init_num(15, 24);
    map_init();

    print_path(NULL, 0);

    uint8_t r, c;
    int16_t *path = NULL, len = 0;

    srand(time(0));
    while (1)
    {
        r = rand() % NUMBER_OF_ROWS;
        c = rand() % ROW_SIZE;
        vTaskDelay(250 / portTICK_PERIOD_MS);
        set_init_num(c, r);
        if (path_find() >= 0)
        {
            printf("Found something for now. Start node %d, end node %d\n", XY_TO_NODE(c, r), XY_TO_NODE(12, 34));
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            path = reconstruct_path(&len);
            if (path != NULL)
            {
                //print_path(path, &len);
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