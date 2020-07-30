cmake_minimum_required(VERSION 3.0)

list(APPEND xpkview_SOURCES
  xpkview.c
)

add_executable(xpkview ${xpkview_SOURCES})

target_include_directories(xpkview PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../xpk
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
  ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(xpkview lpxpk lpclu lputil ${SDL2_LDFLAGS})

########## ########## ##########

install(
  TARGETS xpkview
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
