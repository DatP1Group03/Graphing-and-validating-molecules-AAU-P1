
# toxicphore/CMakeLists.txt

# Lav et statisk library (kan også være SHARED hvis du vil)
add_library(toxicphore
    toxicphore.c
)

# Sørg for at alle, der linker med toxicphore, kan finde headeren
target_include_directories(toxicphore
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(toxicphore
        bfs_matrix
        dfs_matrix
        validation
        valenceCheck
        Adjacency_matrix
        gui
        raylib
        #opengl32 er windows-libs
        #gdi32
        #winmm
)
