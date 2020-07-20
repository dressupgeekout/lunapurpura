cmake_minimum_required(VERSION 3.0)

add_library(lpprx STATIC
  prdprs.c
  prdprs.h
  presagearchive.c
  presagearchive.h
  presagearchivemember.c
  presagearchivemember.h
  prx.c
  prx.h
)

target_include_directories(lpprx PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpprx lputil)

########## ########## ##########

add_executable(prx
  main.c
)

target_include_directories(prx PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(prx lpprx lputil)
