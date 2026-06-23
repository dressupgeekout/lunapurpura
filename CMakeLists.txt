cmake_minimum_required(VERSION 3.0)
project(lunapurpura)

include(GNUInstallDirs)

# User-settable options.
set(LUNAPURPURA_DEBUG ON CACHE BOOL "Enable debug output")
set(LUNAPURPURA_BUILD_LUA_BINDINGS OFF CACHE BOOL "Build the Lua/LuaJIT bindings")
set(LUNAPURPURA_BUILD_MRUBY_BINDINGS OFF CACHE BOOL "Build the mruby bindings")
set(LUNAPURPURA_BUILD_TESTS ON CACHE BOOL "Build the test applications")
set(LUNAPURPURA_PNG_SUPPORT ON CACHE BOOL "Enable conversion to PNG")
set(LUNAPURPURA_SDL_SUPPORT ON CACHE BOOL "Enable apps which require SDL2")

if(${LUNAPURPURA_BUILD_LUA_BINDINGS})
  message(STATUS "Lua bindings enabled")
  set(LUA_INCLUDEDIR "" CACHE PATH "Location of Lua 5.1/LuaJIT's header files")
  set(LUA_LIBDIR "" CACHE PATH "Location of Lua 5.1/LuaJIT's library")
endif()

if(${LUNAPURPURA_BUILD_MRUBY_BINDINGS})
  message(STATUS "mruby bindings enabled")
  set(MRUBY_INCLUDEDIR "" CACHE PATH "Location of mruby's header files")
  set(MRUBY_LIBDIR "" CACHE PATH "Location of mruby's library")
endif()

if(${LUNAPURPURA_BUILD_TESTS})
  message(STATUS "Test programs enabled")
endif()

if(${LUNAPURPURA_PNG_SUPPORT})
  message(STATUS "PNG support enabled")
  set(LUNAPURPURA_PNG_SUPPORT_FLAGS "-DLUNAPURPURA_PNG_SUPPORT")
endif()

include(FindPkgConfig)
if(${LUNAPURPURA_SDL_SUPPORT})
  message(STATUS "SDL support enabled")
  pkg_check_modules(SDL2 REQUIRED sdl2)
endif()

include(CheckFunctionExists)
check_function_exists(getprogname LUNAPURPURA_HAVE_GETPROGNAME)

if(${LUNAPURPURA_HAVE_GETPROGNAME})
  add_definitions(-DLUNAPURPURA_HAVE_GETPROGNAME)
endif()

# Global build parameters.
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall)

if(${LUNAPURPURA_DEBUG})
  message(STATUS "DEBUG macro enabled")
  add_definitions(-DLUNAPURPURA_DEBUG)
endif()

include(TestBigEndian)
test_big_endian(LUNAPURPURA_BIG_ENDIAN)
if(${LUNAPURPURA_BIG_ENDIAN})
  add_definitions(-DLUNAPURPURA_BIG_ENDIAN)
endif()

# Good to go.
add_subdirectory(src)
