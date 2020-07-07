cmake_minimum_required(VERSION 3.0)

list(APPEND LPUTIL_SOURCES
  lputil.c
  lputil.h
)

# This distribution of getopt(3) and friends is based on NetBSD's
# implementation, but it's infused with Windows-specific support.
# Therefore, include these files *because* we're building on Windows, and
# not for any other reason.
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  message(STATUS "Building for Windows")
  list(APPEND LPUTIL_SOURCES
    getopt.c
    getopt.h
  )
endif()

add_library(lputil STATIC ${LPUTIL_SOURCES})

target_include_directories(lputil PRIVATE
   ${CMAKE_CURRENT_LIST_DIR}
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  target_compile_options(lputil PUBLIC "-DLUNAPURPURA_TARGET_WINDOWS")
  target_link_libraries(lputil Ws2_32)
endif()
