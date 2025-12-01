cmake_minimum_required(VERSION 3.20)
project(P1 C)
set(CMAKE_C_STANDARD 11)

add_executable(test_SMILESNODE test_SMILESNODE.c)

target_link_libraries(test_SMILESNODE PRIVATE SMILES_NODE validation)