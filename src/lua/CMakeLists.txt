cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

find_library(LIBLUAJIT NAMES luajit PATHS ${LUAJIT_LIBDIR} REQUIRED)

########## ########## ##########

add_library(luaclu SHARED
  luaclu.c
)

target_include_directories(luaclu PUBLIC
  ${LUAJIT_INCLUDEDIR}
)

target_include_directories(luaclu PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaclu ${LIBLUAJIT} lpclu lputil)

########## ########## ##########

add_library(luaprd SHARED
  luaprd.c
)

target_include_directories(luaprd PUBLIC
  ${LUAJIT_INCLUDEDIR}
)

target_include_directories(luaprd PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../prd
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaprd ${LIBLUAJIT} lpprd lputil)

########## ########## ##########

add_library(luaxpk SHARED
  luaxpk.c
)

target_include_directories(luaxpk PUBLIC
  ${LUAJIT_INCLUDEDIR}
)

target_include_directories(luaxpk PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../xpk
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaxpk ${LIBLUAJIT} lpxpk lputil)
