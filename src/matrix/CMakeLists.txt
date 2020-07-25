cmake_minimum_required(VERSION 3.0)

add_library(lpmatrix STATIC
  matrix.c
  matrix.h
)

target_include_directories(lpmatrix PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpmatrix lputil)

########## ########## ##########

add_executable(matrix
  main.c
)

target_include_directories(matrix PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(matrix lpmatrix lputil)
