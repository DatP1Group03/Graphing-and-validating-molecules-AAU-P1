
add_library(gui gui.c raygui_impl.c draw_text_boxed.c)
add_subdirectory(Graph_representation)

target_include_directories(gui PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/raygui
)

target_link_libraries(gui PUBLIC
  bfs_matrix
  dfs_matrix
  valenceCheck
  Adjacency_matrix
  validation
  toxicphore
  Graph_representation
  raylib
  SMILES_NODE
)
