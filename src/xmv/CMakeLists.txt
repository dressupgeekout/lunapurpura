cmake_minimum_required(VERSION 3.0)

list(APPEND lpxmv_SOURCES
  xmv.c
)

list(APPEND lpxmv_HEADERS
  xmv.h
)

add_library(lpxmv STATIC ${lpxmv_SOURCES} ${lpxmv_HEADERS})

target_include_directories(lpxmv PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpxmv lputil)

########## ########## ##########

list(APPEND xmv_SOURCES
  main.c
)

add_executable(xmv ${xmv_SOURCES})

target_include_directories(xmv PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(xmv lpxmv lputil)

########## ########## ##########

install(
  TARGETS lpxmv xmv
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${lpxmv_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunapurpura
)
