set(srcs)
set(include_dirs "include")

list(APPEND srcs
     )

idf_component_register(SRCS "prio_queue.c" "map.c" "path_finder.c" "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                      )