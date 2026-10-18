set(srcs)
set(include_dirs "include")

list(APPEND srcs
      "path_finding.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "${include_dirs}"
                      )