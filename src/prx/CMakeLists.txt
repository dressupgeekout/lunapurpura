cmake_minimum_required(VERSION 3.0)

list(APPEND lpprx_SOURCES
  prdprs.c
  presagearchive.c
  presagearchivemember.c
  prx.c
)

list(APPEND lpprx_HEADERS
  prdprs.h
  presagearchive.h
  presagearchivemember.h
  prx.h
)

add_library(lpprx STATIC ${lpprx_SOURCES} ${lpprx_HEADERS})

target_include_directories(lpprx PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpprx lputil)

########## ########## ##########

list(APPEND prx_SOURCES
  main.c
)

add_executable(prx ${prx_SOURCES})

target_include_directories(prx PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(prx lpprx lputil)

########## ########## ##########

install(
  TARGETS lpprx prx
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${lpprx_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunapurpura
)
