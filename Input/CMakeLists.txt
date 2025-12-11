
add_library(validation
        validation.c
)

add_library(drawTextBoxed
        drawBoxedText.c
)

target_include_directories(validation PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(drawTextBoxed PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/raylib/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)
