cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

########## ########## ##########

add_library(lpclu STATIC
  clu.c
  clu.h
)

target_include_directories(lpclu PRIVATE  
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

########## ########## ##########

add_executable(clu
  main.c
)

target_include_directories(clu PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(clu lpclu lputil)
