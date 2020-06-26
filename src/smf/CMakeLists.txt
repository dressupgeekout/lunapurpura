cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

add_library(lpsmf STATIC
  smf.c
  smf.h
)

target_include_directories(lpsmf PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpsmf lputil)

########## ########## ##########

add_executable(smf
  main.c
)

target_include_directories(smf PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(smf lpsmf lputil)
