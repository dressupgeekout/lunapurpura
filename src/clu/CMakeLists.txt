cmake_minimum_required(VERSION 3.0)

########## ########## ##########

list(APPEND lpclu_SOURCES
  clu.c
)

list(APPEND lpclu_HEADERS
  clu.h
)

add_library(lpclu STATIC ${lpclu_SOURCES} ${lpclu_HEADERS})

target_include_directories(lpclu PRIVATE  
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

########## ########## ##########

set(clu_SOURCES
  main.c
)

add_executable(clu ${clu_SOURCES})

target_include_directories(clu PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(clu lpclu lputil)

########## ########## ##########

install(
  TARGETS lpclu clu
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${lpclu_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunapurpura
)
