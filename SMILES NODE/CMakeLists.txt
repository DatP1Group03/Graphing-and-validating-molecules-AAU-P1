cmake_minimum_required(VERSION 4.0)
project(Projekt_kode C)

set(CMAKE_C_STANDARD 11)

add_executable("SMILES NODE/SMILESNODE.c")

add_subdirectory(Databehandling)
include(cmake/mtest.cmake)

enable_testing()
target_link_libraries(SMILES NODE/SMILESNODE.c)
add_subdirectory(test)