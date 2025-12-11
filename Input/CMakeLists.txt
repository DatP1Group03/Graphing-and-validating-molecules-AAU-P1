
add_library(validation
        validation.c
)


target_include_directories(validation PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

