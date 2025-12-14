cmake_minimum_required(VERSION 3.10)

add_library(SMILES_NODE SMILESNODE.c SMILESNODE.h)

target_link_libraries(SMILES_NODE PRIVATE validation)

target_include_directories(SMILES_NODE PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
