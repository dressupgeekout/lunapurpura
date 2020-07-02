cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

# User-settable options.
set(LUNAPURPURA_XPK_HACK ON CACHE BOOL "Enable a silly XPK hack")
set(LUNAPURPURA_XPK_TRACE OFF CACHE BOOL "Add tracing output while decoding XPKs")

########## ########## ##########

add_library(lpxpk STATIC
  xpk.c
  xpk.h
  xpkdecoder.c
  xpkdecoder.h
)

if(${LUNAPURPURA_XPK_HACK})
  message(STATUS "XPK hack enabled")
  target_compile_options(lpxpk PUBLIC "-DLUNAPURPURA_XPK_HACK")
endif()

if(${LUNAPURPURA_XPK_TRACE})
  message(STATUS "XPK decode tracing enabled")
  target_compile_options(lpxpk PUBLIC "-DLUNAPURPURA_XPK_TRACE")
endif()

target_include_directories(lpxpk PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
  ${CMAKE_CURRENT_LIST_DIR}/../clu
  "${PNG_INCUDE_DIRS}"
)

target_link_libraries(lpxpk lpclu lputil)

if(${LUNAPURPURA_PNG_SUPPORT})
  target_compile_options(lpxpk PUBLIC "-DLUNAPURPURA_PNG_SUPPORT" "${PNG_DEFINTIONS}")
  target_link_libraries(lpxpk "${PNG_LIBRARIES}")
endif()

########## ########## ##########

add_executable(xpk
  main.c
)

target_include_directories(xpk PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../lputil
  ${CMAKE_CURRENT_LIST_DIR}/../clu
)

target_link_libraries(xpk lpxpk lpclu lputil)

if(${LUNAPURPURA_PNG_SUPPORT})
  target_compile_options(xpk PUBLIC "-DLUNAPURPURA_PNG_SUPPORT" "${PNG_DEFINTIONS}")
  target_link_libraries(xpk "${PNG_LIBRARIES}")
endif()
