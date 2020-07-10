cmake_minimum_required(VERSION 3.0)

add_library(lpriff STATIC
  riff.c
  riff.h
)

target_include_directories(lpriff PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpriff lputil)
