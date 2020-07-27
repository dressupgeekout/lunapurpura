cmake_minimum_required(VERSION 3.0)

add_executable(xpkview
  xpkview.c
)

target_include_directories(xpkview PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../xpk
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(xpkview lpxpk lpclu lputil ${SDL2_LDFLAGS})
