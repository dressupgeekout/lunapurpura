cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

find_library(LIBMRUBY NAMES mruby PATHS ${MRUBY_LIBDIR})

########## ########## ##########

add_library(mrubyclu SHARED
  src/mrubyclu.c
)

target_include_directories(mrubyclu PUBLIC
  ${MRUBY_INCLUDEDIR}
)

target_include_directories(mrubyclu PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(mrubyclu ${LIBMRUBY} lpclu lputil)
