cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

add_library(lputil STATIC
  lputil.c
  lputil.h
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  target_compile_options(lputil PUBLIC "-DLUNAPURPURA_TARGET_WINDOWS")
  target_link_libraries(lputil Ws2_32)
endif()
