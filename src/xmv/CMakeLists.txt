cmake_minimum_required(VERSION 3.0)

add_library(lpxmv STATIC
  xmv.c
  xmv.h
)

target_include_directories(lpxmv PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpxmv lputil)

########## ########## ##########

add_executable(xmv
  main.c
)

target_include_directories(xmv PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(xmv lpxmv lputil)
