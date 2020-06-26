cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

########## ########## ##########

add_library(lpxpk STATIC
  xpk.c
  xpk.h
)

target_include_directories(lpxpk PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
  ${CMAKE_CURRENT_LIST_DIR}/../clu
)

target_link_libraries(lpxpk lpclu lputil)

########## ########## ##########

add_executable(xpk
  main.c
)

target_include_directories(xpk PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
  ${CMAKE_CURRENT_LIST_DIR}/../clu
)

target_link_libraries(xpk lpxpk lpclu lputil)
