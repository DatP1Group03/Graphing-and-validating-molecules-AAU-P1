add_library(valenceCheck valence_check.c)
add_library(bfs_matrix bfs_matrix.c)
add_library(dfs_matrix dfs_matrix.c)

target_include_directories(valenceCheck PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/raylib/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(bfs_matrix PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/raylib/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(dfs_matrix PUBLIC
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/raylib/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(bfs_matrix PUBLIC valenceCheck m)
target_link_libraries(dfs_matrix PUBLIC valenceCheck m)
