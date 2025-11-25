cmake_minimum_required(VERSION 3.20)
project(P1 C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Input)

add_library(SMILES_NODE
        "SMILES NODE/SMILESNODE.c"
        "SMILES NODE/SMILESNODE.h"
)

target_include_directories(SMILES_NODE PUBLIC ${CMAKE_SOURCE_DIR}/Input)
target_link_libraries(SMILES_NODE PRIVATE validation)

add_executable(main
        "SMILES NODE/main.c"
        "test for SMILES NODE/test_SMILESNODE.c"
)


target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/Input)
target_link_libraries(main PRIVATE SMILES_NODE validation)
