cmake_minimum_required(VERSION 3.0)

list(APPEND lpsmf_SOURCES
  smf.c
)

list(APPEND lpsmf_HEADERS
  smf.h
)

add_library(lpsmf STATIC ${lpsmf_SOURCES} ${lpsmf_HEADERS})

target_include_directories(lpsmf PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpsmf lputil)

########## ########## ##########

list(APPEND smf_SOURCES
  main.c
)

add_executable(smf ${smf_SOURCES})

target_include_directories(smf PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(smf lpsmf lputil)

########## ########## ##########

install(
  TARGETS lpsmf smf
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${lpsmf_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunapurpura
)
