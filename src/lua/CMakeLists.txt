cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

find_library(LIBLUA
  NAMES lua lua51 luajit luajit-5.1
  PATHS ${LUA_LIBDIR}
  NO_DEFAULT_PATH
)

########## ########## ##########

add_library(luaclu SHARED
  luaclu.c
)

target_include_directories(luaclu PUBLIC
  ${LUA_INCLUDEDIR}
)

target_include_directories(luaclu PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaclu ${LIBLUA} lpclu lputil)

########## ########## ##########

add_library(luaprd SHARED
  luaprd.c
)

target_include_directories(luaprd PUBLIC
  ${LUA_INCLUDEDIR}
)

target_include_directories(luaprd PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../prd
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaprd ${LIBLUA} lpprd lputil)

########## ########## ##########

add_library(luaprx SHARED
  luaprx.c
)

target_include_directories(luaprx PUBLIC
  ${LUA_INCLUDEDIR}
)

target_include_directories(luaprx PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../prx
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaprx ${LIBLUA} lpprx lputil)


########## ########## ##########

add_library(luaxpk SHARED
  luaxpk.c
)

target_include_directories(luaxpk PUBLIC
  ${LUA_INCLUDEDIR}
)

target_include_directories(luaxpk PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../xpk
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(luaxpk ${LIBLUA} lpxpk lputil)
