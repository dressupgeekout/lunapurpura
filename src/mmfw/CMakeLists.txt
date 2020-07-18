cmake_minimum_required(VERSION 3.0)

add_library(lpmmfw STATIC
  mmfw.c
  mmfw.h
  mmfwfilm.c
  mmfwfilm.h
  mmfwpicture.c
  mmfwpicture.h
  mmfwsound.c
  mmfwsound.h
)

target_include_directories(lpmmfw PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(lpmmfw lputil)

########## ########## ##########

add_executable(mmfw
  main.c
)

target_include_directories(mmfw PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
)

target_link_libraries(mmfw lpmmfw lputil)
