cmake_minimum_required(VERSION 3.0)

########## ########## ##########

add_library(lpprd STATIC
  prd.c
  prd.h
  prdentry.c
  prdentry.h
)

target_include_directories(lpprd PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

########## ########## ##########

add_executable(prd
  main.c
)

target_include_directories(prd PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(prd lpprd lputil)
