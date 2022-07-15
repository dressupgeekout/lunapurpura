cmake_minimum_required(VERSION 3.0)

list(APPEND lpdib_SOURCES
  dib.c
)

list(APPEND lpdib_HEADERS
  dib.h
)

add_library(lpdib STATIC ${lpdib_SOURCES} ${lpdib_HEADERS})

########## ########## ##########

install(
  TARGETS lpdib
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  FILES ${lpdib_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunapurpura
)
