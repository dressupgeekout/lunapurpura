cmake_minimum_required(VERSION 3.0)

list(APPEND lpmmfw_SOURCES
  mmfw.c
  mmfwfilm.c
  mmfwpicture.c
  mmfwsound.c
)

list(APPEND lpmmfw_HEADERS
  mmfw.h
  mmfwfilm.h
  mmfwpicture.h
  mmfwsound.h
)

add_library(lpmmfw STATIC ${lpmmfw_SOURCES} ${lpmmfw_HEADERS})

target_include_directories(lpmmfw PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpmmfw lputil)

########## ########## ##########

list(APPEND mmfw_SOURCES
  main.c
)

add_executable(mmfw ${mmfw_SOURCES})

target_include_directories(mmfw PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(mmfw lpmmfw lputil)

########## ########## ##########

install(
  TARGETS lpmmfw mmfw
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
  FILES ${lpmmfw_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunapurpura
)
